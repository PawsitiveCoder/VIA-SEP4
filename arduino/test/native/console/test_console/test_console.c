#include <fff.h>
#include <unity.h>
#include <stdarg.h>
#include <string.h>
#include "console.h"
#include "uart.h"

DEFINE_FFF_GLOBALS;

FAKE_VOID_FUNC(uart_init, USART_t, uint32_t, UART_Callback_t);
FAKE_VOID_FUNC(uart_send_string_blocking, USART_t, char *);

void setUp(void)
{
    RESET_FAKE(uart_init);
    RESET_FAKE(uart_send_string_blocking);
}

void tearDown(void) {}

void test_console_init_calls_uart_init_once_with_correct_parameters(void)
{
    console_init();
    console_init();
    TEST_ASSERT_EQUAL(1, uart_init_fake.call_count);
    TEST_ASSERT_EQUAL(BUILT_IN_CONSOLE, uart_init_fake.arg0_val);
    TEST_ASSERT_EQUAL(BUILT_IN_CONSOLE_BAUD, uart_init_fake.arg1_val);
    TEST_ASSERT_EQUAL(NULL, uart_init_fake.arg2_val);
}

void test_console_log_sends_formatted_string_to_uart(void)
{
    const char *msg = "Hello %s %d!";

    console_log(msg, "World", 42);

    TEST_ASSERT_EQUAL(1, uart_send_string_blocking_fake.call_count);
    TEST_ASSERT_EQUAL_STRING("Hello World 42!", uart_send_string_blocking_fake.arg1_val);
}

void test_console_log_handles_empty_string(void)
{
    console_log("");
    TEST_ASSERT_EQUAL(1, uart_send_string_blocking_fake.call_count);
    TEST_ASSERT_EQUAL_STRING("", uart_send_string_blocking_fake.arg1_val);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_console_init_calls_uart_init_once_with_correct_parameters);
    RUN_TEST(test_console_log_sends_formatted_string_to_uart);
    RUN_TEST(test_console_log_handles_empty_string);
    return UNITY_END();
}

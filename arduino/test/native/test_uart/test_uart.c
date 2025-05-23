#include "unity.h"
#include "uart.h"
#include "mock_avr_io.h"
#include <fff.h>

// Mock AVR UART registers for test linking
uint8_t UBRR0H, UBRR0L, UBRR1H, UBRR1L, UBRR2H, UBRR2L, UBRR3H, UBRR3L;
uint8_t UCSR0A, UCSR0B, UCSR0C, UCSR1A, UCSR1B, UCSR1C;
uint8_t UCSR2A, UCSR2B, UCSR2C, UCSR3A, UCSR3B, UCSR3C;
uint8_t UDR0, UDR1, UDR2, UDR3;

DEFINE_FFF_GLOBALS;
FAKE_VOID_FUNC(sei);
FAKE_VOID_FUNC(cli);

void setUp(void)
{
  UBRR0H = UBRR0L = UBRR1H = UBRR1L = UBRR2H = UBRR2L = UBRR3H = UBRR3L = 0;
  UCSR0A = UCSR0B = UCSR0C = UCSR1A = UCSR1B = UCSR1C = 0;
  UCSR2A = UCSR2B = UCSR2C = UCSR3A = UCSR3B = UCSR3C = 0;
  UDR0 = UDR1 = UDR2 = UDR3 = 0;

  RESET_FAKE(sei);
  RESET_FAKE(cli);
}

void tearDown(void)
{
  // clean stuff up here
}

void test_uart_init0()
{
  uart_init(USART_0, 9600, NULL);
}

void test_uart_init1()
{
  uart_init(USART_1, 9600, NULL);
}

void test_uart_init2()
{
  uart_init(USART_2, 9600, NULL);
}

void test_uart_init3()
{
  uart_init(USART_3, 9600, NULL);
}

int main(void)
{
  UNITY_BEGIN();
  RUN_TEST(test_uart_init0);
  RUN_TEST(test_uart_init1);
  RUN_TEST(test_uart_init2);
  RUN_TEST(test_uart_init3);
  return UNITY_END();
}

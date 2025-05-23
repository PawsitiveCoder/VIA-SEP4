#include <fff.h>
#include <unity.h>
#include <string.h>
#include "console_observer.h"
#include "console.h"

DEFINE_FFF_GLOBALS;

FAKE_VOID_FUNC(console_init);
FAKE_VOID_FUNC_VARARG(console_log, const char *, ...);
FAKE_VALUE_FUNC(observer_t, observer_create, observer_callback_t);
FAKE_VOID_FUNC(observer_destroy, observer_t);

static observer_t fake_observer = (observer_t)0x1234;

void setUp(void)
{
  RESET_FAKE(console_init);
  RESET_FAKE(console_log);
  RESET_FAKE(observer_create);
  RESET_FAKE(observer_destroy);
  observer_create_fake.return_val = fake_observer;
}

void tearDown(void) {}

void test_console_observer_create_calls_observer_create_and_console_init(void)
{
  observer_t obs = console_observer_create();
  TEST_ASSERT_EQUAL_PTR(fake_observer, obs);
  TEST_ASSERT_EQUAL(1, observer_create_fake.call_count);
  TEST_ASSERT_EQUAL(1, console_init_fake.call_count);
}

void test_console_observer_create_returns_null_if_observer_create_fails(void)
{
  observer_create_fake.return_val = NULL;
  observer_t obs = console_observer_create();
  TEST_ASSERT_NULL(obs);
  TEST_ASSERT_EQUAL(1, observer_create_fake.call_count);
  TEST_ASSERT_EQUAL(0, console_init_fake.call_count);
}

void test_console_observer_destroy_calls_observer_destroy_when_not_null(void)
{
  console_observer_destroy(fake_observer);
  TEST_ASSERT_EQUAL(1, observer_destroy_fake.call_count);
  TEST_ASSERT_EQUAL_PTR(fake_observer, observer_destroy_fake.arg0_val);
}

void test_console_observer_destroy_does_nothing_when_null(void)
{
  console_observer_destroy(NULL);
  TEST_ASSERT_EQUAL(0, observer_destroy_fake.call_count);
}

void test_console_observer_callback_calls_console_log_with_expected_format(void)
{
  observer_callback_t console_observer_callback = NULL;
  console_observer_create(); // This will set observer_create_fake.arg0_val
  console_observer_callback = observer_create_fake.arg0_val;

  TEST_ASSERT_NOT_NULL(console_observer_callback);

  // Simulate notification
  console_observer_callback("temperature", "25", "C");

  TEST_ASSERT_EQUAL(1, console_log_fake.call_count);
  TEST_ASSERT_EQUAL_STRING("Sensor: %s, Value: %s, Unit: %s\n", console_log_fake.arg0_val);
}

int main(void)
{
  UNITY_BEGIN();
  RUN_TEST(test_console_observer_create_calls_observer_create_and_console_init);
  RUN_TEST(test_console_observer_create_returns_null_if_observer_create_fails);
  RUN_TEST(test_console_observer_destroy_calls_observer_destroy_when_not_null);
  RUN_TEST(test_console_observer_destroy_does_nothing_when_null);
  RUN_TEST(test_console_observer_callback_calls_console_log_with_expected_format);
  return UNITY_END();
}

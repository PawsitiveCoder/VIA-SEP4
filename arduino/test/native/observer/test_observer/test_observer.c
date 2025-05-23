#include <fff.h>
#include <unity.h>
#include <stdlib.h>
#include <string.h>
#include "observer.h"

DEFINE_FFF_GLOBALS;

FAKE_VOID_FUNC(observer_callback_fake, const char *, const char *, const char *);

void setUp(void)
{
    RESET_FAKE(observer_callback_fake);
}

void tearDown(void) {}

void test_observer_create_returns_non_null_for_valid_callback(void)
{
    observer_t observer = observer_create(observer_callback_fake);
    TEST_ASSERT_NOT_NULL(observer);
    observer_destroy(observer);
}

void test_observer_create_returns_null_for_null_callback(void)
{
    observer_t observer = observer_create(NULL);
    TEST_ASSERT_NULL(observer);
    observer_destroy(observer);
}

void test_observer_destroy_handles_null_pointer(void)
{
    observer_destroy(NULL);
}

void test_observer_notify_calls_callback_with_correct_args(void)
{
    observer_t observer = observer_create(observer_callback_fake);
    const char *topic = "topic";
    const char *value1 = "v1";
    const char *value2 = "v2";

    observer_notify(observer, topic, value1, value2);
    TEST_ASSERT_EQUAL(1, observer_callback_fake_fake.call_count);
    TEST_ASSERT_EQUAL_STRING(topic, observer_callback_fake_fake.arg0_val);
    TEST_ASSERT_EQUAL_STRING(value1, observer_callback_fake_fake.arg1_val);
    TEST_ASSERT_EQUAL_STRING(value2, observer_callback_fake_fake.arg2_val);

    observer_destroy(observer);
}

void test_observer_notify_does_nothing_if_observer_is_null(void)
{
    observer_notify(NULL, "topic", "v1", "v2");
    TEST_ASSERT_EQUAL(0, observer_callback_fake_fake.call_count);
}

void test_observer_notify_does_nothing_if_callback_is_null(void)
{
    observer_t observer = observer_create(NULL);

    observer_notify(observer, "topic", "v1", "v2");
    TEST_ASSERT_EQUAL(0, observer_callback_fake_fake.call_count);

    observer_destroy(observer);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_observer_create_returns_non_null_for_valid_callback);
    RUN_TEST(test_observer_create_returns_null_for_null_callback);
    RUN_TEST(test_observer_destroy_handles_null_pointer);
    RUN_TEST(test_observer_notify_calls_callback_with_correct_args);
    RUN_TEST(test_observer_notify_does_nothing_if_observer_is_null);
    RUN_TEST(test_observer_notify_does_nothing_if_callback_is_null);
    return UNITY_END();
}

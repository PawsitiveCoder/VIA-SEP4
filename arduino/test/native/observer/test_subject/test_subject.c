#include <fff.h>
#include <unity.h>
#include <stdlib.h>
#include <string.h>
#include "subject.h"
#include "observer.h"

DEFINE_FFF_GLOBALS;

FAKE_VOID_FUNC(observer_callback_fake, const char *, const char *, const char *);

FAKE_VALUE_FUNC(observer_t, observer_create, observer_callback_t);
FAKE_VOID_FUNC(observer_destroy, observer_t);
FAKE_VOID_FUNC(observer_notify, observer_t, const char *, const char *, const char *);

void setUp(void)
{
    RESET_FAKE(observer_callback_fake);
    RESET_FAKE(observer_create);
    RESET_FAKE(observer_destroy);
    RESET_FAKE(observer_notify);
}

void tearDown(void) {}

void test_subject_create_returns_non_null(void)
{
    subject_t subj = subject_create();
    TEST_ASSERT_NOT_NULL(subj);
    subject_destroy(subj);
}

void test_subject_destroy_handles_null(void)
{
    subject_destroy(NULL);
}

void test_subject_add_observer_adds_once(void)
{
    subject_t subject = subject_create();
    observer_t fake_observer = (observer_t)0x1;

    subject_add_observer(subject, fake_observer);
    subject_add_observer(subject, fake_observer);
    TEST_ASSERT_EQUAL(1, subject_get_observers_count(subject));

    subject_destroy(subject);
}

void test_subject_add_observer_does_not_exceed_max(void)
{
    subject_t subject = subject_create();
    observer_t observers[MAX_OBSERVERS + 2];

    for (int i = 0; i < MAX_OBSERVERS + 2; i++)
    {
        observers[i] = (observer_t)(uintptr_t)(0x1000 + i);
        subject_add_observer(subject, observers[i]);
    }
    TEST_ASSERT_EQUAL(MAX_OBSERVERS, subject_get_observers_count(subject));

    subject_destroy(subject);
}

void test_subject_add_observer_handles_null(void)
{
    subject_t subject = subject_create();
    observer_t fake_observer = (observer_t)0x1;

    subject_add_observer(NULL, NULL);
    subject_add_observer(NULL, fake_observer);
    subject_add_observer(subject, NULL);
    TEST_ASSERT_EQUAL(0, subject_get_observers_count(subject));

    subject_destroy(subject);
}

void test_subject_remove_observer_removes_correctly(void)
{
    subject_t subject = subject_create();
    observer_t fake_observer = (observer_t)0x1;

    subject_add_observer(subject, fake_observer);
    subject_remove_observer(subject, NULL);
    subject_remove_observer(NULL, fake_observer);
    subject_remove_observer(subject, fake_observer);
    TEST_ASSERT_EQUAL(0, subject_get_observers_count(subject));

    subject_destroy(subject);
}

void test_subject_notify_all_calls_observer_notify_for_each(void)
{
    subject_t subject = subject_create();
    observer_t fake_observer_1 = (observer_t)0x1;
    observer_t fake_observer_2 = (observer_t)0x2;

    subject_add_observer(subject, fake_observer_1);
    subject_add_observer(subject, fake_observer_2);
    subject_notify_all(subject, "topic", "v1", "v2");

    TEST_ASSERT_EQUAL(2, observer_notify_fake.call_count);
    TEST_ASSERT_EQUAL_PTR(fake_observer_1, observer_notify_fake.arg0_history[0]);
    TEST_ASSERT_EQUAL_PTR(fake_observer_2, observer_notify_fake.arg0_history[1]);
    TEST_ASSERT_EQUAL_STRING("topic", observer_notify_fake.arg1_history[0]);
    TEST_ASSERT_EQUAL_STRING("v1", observer_notify_fake.arg2_history[0]);
    TEST_ASSERT_EQUAL_STRING("v2", observer_notify_fake.arg3_history[0]);

    subject_destroy(subject);
}

void test_subject_notify_all_handles_null_subject(void)
{
    subject_notify_all(NULL, "topic", "v1", "v2");
    TEST_ASSERT_EQUAL(0, observer_notify_fake.call_count);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_subject_create_returns_non_null);
    RUN_TEST(test_subject_destroy_handles_null);
    RUN_TEST(test_subject_add_observer_adds_once);
    RUN_TEST(test_subject_add_observer_does_not_exceed_max);
    RUN_TEST(test_subject_add_observer_handles_null);
    RUN_TEST(test_subject_remove_observer_removes_correctly);
    RUN_TEST(test_subject_notify_all_calls_observer_notify_for_each);
    RUN_TEST(test_subject_notify_all_handles_null_subject);
    return UNITY_END();
}

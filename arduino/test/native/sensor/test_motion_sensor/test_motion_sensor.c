#include <unity.h>
#include <fff.h>
#include <string.h>
#include <stdlib.h>
#include "motion_sensor.h"
#include "pir.h"

DEFINE_FFF_GLOBALS;

FAKE_VOID_FUNC(pir_init, pir_callback_t);

void setUp(void)
{
    RESET_FAKE(pir_init);
}

void tearDown(void)
{
}

void test_motion_sensor_create_initializes_sensor_and_calls_pir_init(void)
{
    motion_sensor_t sensor = motion_sensor_create();

    TEST_ASSERT_NOT_NULL(sensor);
    TEST_ASSERT_EQUAL(1, pir_init_fake.call_count);
    TEST_ASSERT_NOT_NULL(pir_init_fake.arg0_val);

    motion_sensor_destroy(sensor);
}

void test_motion_sensor_destroy_null_safe(void)
{
    // No crash means success
    motion_sensor_destroy(NULL);
}

void test_motion_sensor_get_data_null_args_safe(void)
{
    motion_sensor_t sensor = motion_sensor_create();
    float value = -1.00;
    char unit[10] = "test";

    // Value and unit should not be modified
    motion_sensor_get_data(NULL, &value, unit);
    motion_sensor_get_data(sensor, NULL, unit);
    motion_sensor_get_data(sensor, &value, NULL);
    TEST_ASSERT_EQUAL_FLOAT(-1.00, value);
    TEST_ASSERT_EQUAL_STRING("test", unit);

    motion_sensor_destroy(sensor);
}

void test_motion_sensor_get_data_reads_and_resets_value(void)
{
    motion_sensor_t sensor = motion_sensor_create();
    float value = -1.00;
    char unit[10] = "test";

    // Simulate motion detected via callback
    pir_init_fake.arg0_val();

    motion_sensor_get_data(sensor, &value, unit);
    TEST_ASSERT_EQUAL_FLOAT(1.00, value);
    TEST_ASSERT_EQUAL_STRING(MOTION_UNIT, unit);

    // Value should be reset to 0 after reading it
    // and no motion event occured in the meantime
    motion_sensor_get_data(sensor, &value, unit);
    TEST_ASSERT_EQUAL_FLOAT(0.00, value);
    TEST_ASSERT_EQUAL_STRING(MOTION_UNIT, unit);

    motion_sensor_destroy(sensor);
}

void test_motion_sensor_get_data_returns_error_value_when_no_motion(void)
{
    motion_sensor_t sensor = motion_sensor_create();
    float value = -1.00;
    char unit[10] = "test";

    // No motion event occured, should return initial ERROR_VALUE
    motion_sensor_get_data(sensor, &value, unit);
    TEST_ASSERT_EQUAL_FLOAT(ERROR_VALUE, value);
    TEST_ASSERT_EQUAL_STRING(MOTION_UNIT, unit);

    motion_sensor_destroy(sensor);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_motion_sensor_create_initializes_sensor_and_calls_pir_init);
    RUN_TEST(test_motion_sensor_destroy_null_safe);
    RUN_TEST(test_motion_sensor_get_data_null_args_safe);
    RUN_TEST(test_motion_sensor_get_data_reads_and_resets_value);
    RUN_TEST(test_motion_sensor_get_data_returns_error_value_when_no_motion);
    return UNITY_END();
}

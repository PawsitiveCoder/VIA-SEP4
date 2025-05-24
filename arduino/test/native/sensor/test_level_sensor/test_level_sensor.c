#include <unity.h>
#include <fff.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "level_sensor.h"
#include "hc_sr04.h"

DEFINE_FFF_GLOBALS;

FAKE_VOID_FUNC(hc_sr04_init);
FAKE_VALUE_FUNC(uint16_t, hc_sr04_takeMeasurement);

void setUp(void)
{
    RESET_FAKE(hc_sr04_init);
    RESET_FAKE(hc_sr04_takeMeasurement);
}

void tearDown(void)
{
}

void test_level_sensor_create_initializes_sensor_and_calls_hc_sr04_init(void)
{
    level_sensor_t sensor = level_sensor_create();

    TEST_ASSERT_NOT_NULL(sensor);
    TEST_ASSERT_EQUAL(1, hc_sr04_init_fake.call_count);

    level_sensor_destroy(sensor);
}

void test_level_sensor_destroy_null_safe(void)
{
    // No crash means success
    level_sensor_destroy(NULL);
}

void test_level_sensor_get_data_null_args_safe(void)
{
    level_sensor_t sensor = level_sensor_create();
    float value = -1.0f;
    char unit[10] = "test";

    // Value and unit should not be modified
    level_sensor_get_data(NULL, &value, unit);
    level_sensor_get_data(sensor, NULL, unit);
    level_sensor_get_data(sensor, &value, NULL);
    TEST_ASSERT_EQUAL_FLOAT(-1.0f, value);
    TEST_ASSERT_EQUAL_STRING("test", unit);

    level_sensor_destroy(sensor);
}

void test_level_sensor_get_data_reads_and_computes_level(void)
{
    level_sensor_t sensor = level_sensor_create();
    float value = -1.00;
    char unit[10] = "test";

    // Simulate a valid measurement
    hc_sr04_takeMeasurement_fake.return_val = 100;
    level_sensor_get_data(sensor, &value, unit);

    TEST_ASSERT_EQUAL_FLOAT(WATER_LEVEL_MOUNT_HEIGHT - 100.00, value);
    TEST_ASSERT_EQUAL_STRING(WATER_LEVEL_UNIT, unit);

    level_sensor_destroy(sensor);
}

void test_level_sensor_get_data_returns_error_value_when_measurement_error(void)
{
    level_sensor_t sensor = level_sensor_create();
    float value = -1.00;
    char unit[10] = "test";

    // Simulate a measurement greater than mount height
    hc_sr04_takeMeasurement_fake.return_val = WATER_LEVEL_MOUNT_HEIGHT + 50;
    level_sensor_get_data(sensor, &value, unit);

    TEST_ASSERT_EQUAL_FLOAT(ERROR_VALUE, value);
    TEST_ASSERT_EQUAL_STRING(WATER_LEVEL_UNIT, unit);

    level_sensor_destroy(sensor);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_level_sensor_create_initializes_sensor_and_calls_hc_sr04_init);
    RUN_TEST(test_level_sensor_destroy_null_safe);
    RUN_TEST(test_level_sensor_get_data_null_args_safe);
    RUN_TEST(test_level_sensor_get_data_reads_and_computes_level);
    RUN_TEST(test_level_sensor_get_data_returns_error_value_when_measurement_error);
    return UNITY_END();
}

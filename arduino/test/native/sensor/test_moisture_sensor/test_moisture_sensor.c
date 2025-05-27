#include <unity.h>
#include <fff.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "moisture_sensor.h"
#include "soil.h"

DEFINE_FFF_GLOBALS;

FAKE_VOID_FUNC(soil_init);
FAKE_VALUE_FUNC(uint16_t, soil_read);

void setUp(void)
{
    RESET_FAKE(soil_init);
    RESET_FAKE(soil_read);
}

void tearDown(void)
{
}

void test_moisture_sensor_create_initializes_sensor_and_calls_soil_init(void)
{
    moisture_sensor_t sensor = moisture_sensor_create();

    TEST_ASSERT_NOT_NULL(sensor);
    TEST_ASSERT_EQUAL(1, soil_init_fake.call_count);

    moisture_sensor_destroy(sensor);
}

void test_moisture_sensor_destroy_null_safe(void)
{
    // No crash means success
    moisture_sensor_destroy(NULL);
}

void test_moisture_sensor_get_data_null_args_safe(void)
{
    moisture_sensor_t sensor = moisture_sensor_create();
    float value = -1.0f;
    char unit[10] = "test";

    // Value and unit should not be modified
    moisture_sensor_get_data(NULL, &value, unit);
    moisture_sensor_get_data(sensor, NULL, unit);
    moisture_sensor_get_data(sensor, &value, NULL);
    TEST_ASSERT_EQUAL_FLOAT(-1.0f, value);
    TEST_ASSERT_EQUAL_STRING("test", unit);

    moisture_sensor_destroy(sensor);
}

void test_moisture_sensor_get_data_reads_and_computes_0_percent_when_dry(void)
{
    moisture_sensor_t sensor = moisture_sensor_create();
    float value = -1.0f;
    char unit[10] = "test";

    soil_read_fake.return_val = 534; // dry_value
    moisture_sensor_get_data(sensor, &value, unit);

    TEST_ASSERT_EQUAL_FLOAT(0.0f, value);
    TEST_ASSERT_EQUAL_STRING(MOISTURE_UNIT, unit);

    moisture_sensor_destroy(sensor);
}

void test_moisture_sensor_get_data_reads_and_computes_100_percent_when_wet(void)
{
    moisture_sensor_t sensor = moisture_sensor_create();
    float value = -1.0f;
    char unit[10] = "test";

    soil_read_fake.return_val = 823; // wet_value
    moisture_sensor_get_data(sensor, &value, unit);

    TEST_ASSERT_EQUAL_FLOAT(100.0f, value);
    TEST_ASSERT_EQUAL_STRING(MOISTURE_UNIT, unit);

    moisture_sensor_destroy(sensor);
}

void test_moisture_sensor_get_data_reads_and_computes_below_dry_value(void)
{
    moisture_sensor_t sensor = moisture_sensor_create();
    float value = -1.0f;
    char unit[10] = "test";

    soil_read_fake.return_val = 100; // below dry_value
    moisture_sensor_get_data(sensor, &value, unit);

    TEST_ASSERT_EQUAL_FLOAT(0.0f, value);
    TEST_ASSERT_EQUAL_STRING(MOISTURE_UNIT, unit);

    moisture_sensor_destroy(sensor);
}

void test_moisture_sensor_get_data_reads_and_computes_above_wet_value(void)
{
    moisture_sensor_t sensor = moisture_sensor_create();
    float value = -1.0f;
    char unit[10] = "test";

    soil_read_fake.return_val = 900; // above wet_value
    moisture_sensor_get_data(sensor, &value, unit);

    TEST_ASSERT_EQUAL_FLOAT(100.0f, value);
    TEST_ASSERT_EQUAL_STRING(MOISTURE_UNIT, unit);

    moisture_sensor_destroy(sensor);
}

void test_moisture_sensor_get_data_reads_and_computes_linear_value(void)
{
    moisture_sensor_t sensor = moisture_sensor_create();
    float value = -1.0f;
    char unit[10] = "test";

    // Value halfway between dry and wet
    soil_read_fake.return_val = (534 + 823) / 2;
    moisture_sensor_get_data(sensor, &value, unit);

    TEST_ASSERT_FLOAT_WITHIN(0.5f, 50.0f, value);
    TEST_ASSERT_EQUAL_STRING(MOISTURE_UNIT, unit);

    moisture_sensor_destroy(sensor);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_moisture_sensor_create_initializes_sensor_and_calls_soil_init);
    RUN_TEST(test_moisture_sensor_destroy_null_safe);
    RUN_TEST(test_moisture_sensor_get_data_null_args_safe);
    RUN_TEST(test_moisture_sensor_get_data_reads_and_computes_0_percent_when_dry);
    RUN_TEST(test_moisture_sensor_get_data_reads_and_computes_100_percent_when_wet);
    RUN_TEST(test_moisture_sensor_get_data_reads_and_computes_below_dry_value);
    RUN_TEST(test_moisture_sensor_get_data_reads_and_computes_above_wet_value);
    RUN_TEST(test_moisture_sensor_get_data_reads_and_computes_linear_value);
    return UNITY_END();
}

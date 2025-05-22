// filepath: /workspace/arduino/test/native/sensor/test_sensor_manager/test_sensor_manager.c
#include <fff.h>
#include <unity.h>
#include <stdlib.h>
#include <string.h>
#include "sensor_manager.h"
#include "subject.h"
#include "temperature_humidity_sensor.h"
#include "moisture_sensor.h"
#include "light_sensor.h"
#include "motion_sensor.h"
#include "stdio.h"

DEFINE_FFF_GLOBALS;

FAKE_VALUE_FUNC(subject_t, subject_create);
FAKE_VOID_FUNC(subject_destroy, subject_t);
FAKE_VOID_FUNC(subject_add_observer, subject_t, observer_t);
FAKE_VOID_FUNC(subject_notify_all, subject_t, const char *, const char *, const char *);

// Temperature and Humidity Sensor
FAKE_VALUE_FUNC(temperature_humidity_sensor_t, temperature_humidity_sensor_create);
FAKE_VOID_FUNC(temperature_humidity_sensor_destroy, temperature_humidity_sensor_t);
FAKE_VOID_FUNC(temperature_humidity_sensor_get_data, temperature_humidity_sensor_t, float *, char *, float *, char *);

// Moisture Sensor
FAKE_VALUE_FUNC(moisture_sensor_t, moisture_sensor_create);
FAKE_VOID_FUNC(moisture_sensor_destroy, moisture_sensor_t);
FAKE_VOID_FUNC(moisture_sensor_get_data, moisture_sensor_t, float *, char *);

// Light Sensor
FAKE_VALUE_FUNC(light_sensor_t, light_sensor_create);
FAKE_VOID_FUNC(light_sensor_destroy, light_sensor_t);
FAKE_VOID_FUNC(light_sensor_get_data, light_sensor_t, float *, char *);

// Motion Sensor
FAKE_VALUE_FUNC(motion_sensor_t, motion_sensor_create);
FAKE_VOID_FUNC(motion_sensor_destroy, motion_sensor_t);
FAKE_VOID_FUNC(motion_sensor_get_data, motion_sensor_t, float *, char *);

void setUp(void)
{
  RESET_FAKE(subject_create);
  subject_create_fake.return_val = (subject_t)0x1;
  RESET_FAKE(subject_destroy);
  RESET_FAKE(subject_add_observer);
  RESET_FAKE(subject_notify_all);

  // Temperature and Humidity Sensor
  RESET_FAKE(temperature_humidity_sensor_create);
  temperature_humidity_sensor_create_fake.return_val = (temperature_humidity_sensor_t)0x2;
  RESET_FAKE(temperature_humidity_sensor_destroy);
  RESET_FAKE(temperature_humidity_sensor_get_data);

  // Moisture Sensor
  RESET_FAKE(moisture_sensor_create);
  moisture_sensor_create_fake.return_val = (moisture_sensor_t)0x3;
  RESET_FAKE(moisture_sensor_destroy);
  RESET_FAKE(moisture_sensor_get_data);

  // Light Sensor
  RESET_FAKE(light_sensor_create);
  light_sensor_create_fake.return_val = (light_sensor_t)0x4;
  RESET_FAKE(light_sensor_destroy);
  RESET_FAKE(light_sensor_get_data);

  // Motion Sensor
  RESET_FAKE(motion_sensor_create);
  motion_sensor_create_fake.return_val = (motion_sensor_t)0x5;
  RESET_FAKE(motion_sensor_destroy);
  RESET_FAKE(motion_sensor_get_data);
}

void tearDown(void) {}

void test_sensor_manager_create_initializes_subject(void)
{
  sensor_manager_t sensor_manager = sensor_manager_create();

  TEST_ASSERT_NOT_NULL(sensor_manager);
  TEST_ASSERT_EQUAL(1, subject_create_fake.call_count);

  sensor_manager_destroy(sensor_manager);
}

void test_sensor_manager_create_initializes_sensors(void)
{
  sensor_manager_t sensor_manager = sensor_manager_create();

  TEST_ASSERT_NOT_NULL(sensor_manager);
  TEST_ASSERT_EQUAL(1, temperature_humidity_sensor_create_fake.call_count);
  TEST_ASSERT_EQUAL(1, moisture_sensor_create_fake.call_count);
  TEST_ASSERT_EQUAL(1, light_sensor_create_fake.call_count);
  TEST_ASSERT_EQUAL(1, motion_sensor_create_fake.call_count);

  sensor_manager_destroy(sensor_manager);
}

void test_sensor_manager_destroy_handles_null(void)
{
  sensor_manager_destroy(NULL);
}

void test_sensor_manager_destroy_calls_all_destroys(void)
{
  sensor_manager_t sensor_manager = sensor_manager_create();
  sensor_manager_destroy(sensor_manager);

  TEST_ASSERT_EQUAL(1, subject_destroy_fake.call_count);
  TEST_ASSERT_EQUAL(1, temperature_humidity_sensor_destroy_fake.call_count);
  TEST_ASSERT_EQUAL(1, moisture_sensor_destroy_fake.call_count);
  TEST_ASSERT_EQUAL(1, light_sensor_destroy_fake.call_count);
  TEST_ASSERT_EQUAL(1, motion_sensor_destroy_fake.call_count);
}

void test_sensor_manager_add_observer_calls_subject_add_observer(void)
{
  sensor_manager_t sensor_manager = sensor_manager_create();
  observer_t observer = (observer_t)0x1;

  sensor_manager_add_observer(sensor_manager, observer);
  TEST_ASSERT_EQUAL(1, subject_add_observer_fake.call_count);
  TEST_ASSERT_EQUAL_PTR(observer, subject_add_observer_fake.arg1_val);

  sensor_manager_destroy(sensor_manager);
}

void test_sensor_manager_read_gets_data_and_notifies(void)
{
  sensor_manager_t sensor_manager = sensor_manager_create();

  sensor_manager_read(sensor_manager);
  TEST_ASSERT_EQUAL(5, subject_notify_all_fake.call_count);

  // Temperature and Humidity Sensor
  TEST_ASSERT_EQUAL(1, temperature_humidity_sensor_get_data_fake.call_count);
  TEST_ASSERT_EQUAL_STRING("temperature", subject_notify_all_fake.arg1_history[0]);
  TEST_ASSERT_EQUAL_STRING("humidity", subject_notify_all_fake.arg1_history[1]);

  // Moisture Sensor
  TEST_ASSERT_EQUAL(1, moisture_sensor_get_data_fake.call_count);
  TEST_ASSERT_EQUAL_STRING("moisture", subject_notify_all_fake.arg1_history[2]);

  // Light Sensor
  TEST_ASSERT_EQUAL(1, light_sensor_get_data_fake.call_count);
  TEST_ASSERT_EQUAL_STRING("light_level", subject_notify_all_fake.arg1_history[3]);

  // Motion Sensor
  TEST_ASSERT_EQUAL(1, motion_sensor_get_data_fake.call_count);
  TEST_ASSERT_EQUAL_STRING("motion", subject_notify_all_fake.arg1_history[4]);

  sensor_manager_destroy(sensor_manager);
}

int main(void)
{
  UNITY_BEGIN();
  RUN_TEST(test_sensor_manager_create_initializes_subject);
  RUN_TEST(test_sensor_manager_create_initializes_sensors);
  RUN_TEST(test_sensor_manager_destroy_handles_null);
  RUN_TEST(test_sensor_manager_destroy_calls_all_destroys);
  RUN_TEST(test_sensor_manager_add_observer_calls_subject_add_observer);
  RUN_TEST(test_sensor_manager_read_gets_data_and_notifies);
  return UNITY_END();
}

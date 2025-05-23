#include <fff.h>
#include <unity.h>
#include <string.h>
#include <stdint.h>
#include "mqtt_observer.h"
#include "observer.h"
#include "mqtt.h"

DEFINE_FFF_GLOBALS;

FAKE_VALUE_FUNC(observer_t, observer_create, observer_callback_t);
FAKE_VOID_FUNC(observer_destroy, observer_t);
FAKE_VALUE_FUNC(MQTT_ERROR_t, mqtt_publish, MQTT_Client_t *, char *, char *, uint16_t);

static char captured_topic[64];
static char captured_message[128];

MQTT_ERROR_t mqtt_publish_custom_fake(MQTT_Client_t *_c, char *topic, char *message, uint16_t _l)
{
  captured_topic[0] = '\0';
  captured_message[0] = '\0';

  if (topic)
  {
    strncpy(captured_topic, topic, sizeof(captured_topic) - 1);
    captured_topic[sizeof(captured_topic) - 1] = '\0'; // Ensure null-termination
  }

  if (message)
  {
    strncpy(captured_message, message, sizeof(captured_message) - 1);
    captured_message[sizeof(captured_message) - 1] = '\0'; // Ensure null-termination
  }

  return mqtt_publish_fake.return_val;
}

static observer_t fake_observer = (observer_t)0x1234;
static MQTT_Client_t fake_client;

void setUp(void)
{
  RESET_FAKE(observer_create);
  RESET_FAKE(observer_destroy);
  RESET_FAKE(mqtt_publish);
  memset(captured_topic, 0, sizeof(captured_topic));
  memset(captured_message, 0, sizeof(captured_message));
  mqtt_publish_fake.custom_fake = mqtt_publish_custom_fake;
  observer_create_fake.return_val = fake_observer;
  memset(&fake_client, 0, sizeof(fake_client));
}

void tearDown(void) {}

void test_mqtt_observer_create_calls_observer_create_and_copies_client(void)
{
  observer_t observer = mqtt_observer_create(&fake_client);

  TEST_ASSERT_EQUAL_PTR(fake_observer, observer);
  TEST_ASSERT_EQUAL(1, observer_create_fake.call_count);

  mqtt_observer_destroy(observer);
}

void test_mqtt_observer_create_returns_null_if_observer_create_fails(void)
{
  observer_create_fake.return_val = NULL;
  observer_t observer = mqtt_observer_create(&fake_client);

  TEST_ASSERT_EQUAL(1, observer_create_fake.call_count);
  TEST_ASSERT_NULL(observer);

  mqtt_observer_destroy(observer);
}

void test_mqtt_observer_destroy_calls_observer_destroy_when_not_null(void)
{
  mqtt_observer_destroy(fake_observer);
  TEST_ASSERT_EQUAL(1, observer_destroy_fake.call_count);
  TEST_ASSERT_EQUAL_PTR(fake_observer, observer_destroy_fake.arg0_val);
}

void test_mqtt_observer_destroy_does_nothing_when_null(void)
{
  mqtt_observer_destroy(NULL);
  TEST_ASSERT_EQUAL(0, observer_destroy_fake.call_count);
}

void test_mqtt_observer_callback_calls_mqtt_publish_with_expected_topic_and_message(void)
{
  observer_callback_t mqtt_observer_callback = NULL;
  observer_t observer = mqtt_observer_create(&fake_client);
  mqtt_observer_callback = observer_create_fake.arg0_val;

  TEST_ASSERT_NOT_NULL(mqtt_observer_callback);

  // Simulate notification
  mqtt_observer_callback("humidity", "55", "%");

  TEST_ASSERT_EQUAL(1, mqtt_publish_fake.call_count);
  TEST_ASSERT_EQUAL_STRING("greenhouse/humidity", captured_topic);
  TEST_ASSERT_EQUAL_STRING("{\"value\":\"55\",\"unit\":\"%\"}", captured_message);

  mqtt_observer_destroy(observer);
}

int main(void)
{
  UNITY_BEGIN();
  RUN_TEST(test_mqtt_observer_create_calls_observer_create_and_copies_client);
  RUN_TEST(test_mqtt_observer_create_returns_null_if_observer_create_fails);
  RUN_TEST(test_mqtt_observer_destroy_calls_observer_destroy_when_not_null);
  RUN_TEST(test_mqtt_observer_destroy_does_nothing_when_null);
  RUN_TEST(test_mqtt_observer_callback_calls_mqtt_publish_with_expected_topic_and_message);
  return UNITY_END();
}

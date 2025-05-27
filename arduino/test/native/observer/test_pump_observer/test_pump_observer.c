#include <fff.h>
#include <unity.h>
#include <string.h>
#include "pump_observer.h"
#include "observer.h"
#include "relay.h"
#include "tone.h"

DEFINE_FFF_GLOBALS;

// Fakes for relay
FAKE_VALUE_FUNC(relay_t, relay_create, volatile uint8_t *, uint8_t);
FAKE_VOID_FUNC(relay_destroy, relay_t);
FAKE_VOID_FUNC(relay_on, relay_t);
FAKE_VOID_FUNC(relay_off, relay_t);
FAKE_VALUE_FUNC(uint8_t, relay_is_on, relay_t);

// Fakes for observer
FAKE_VALUE_FUNC(observer_t, observer_create, observer_callback_t);
FAKE_VOID_FUNC(observer_destroy, observer_t);

// Fakes for tone
FAKE_VOID_FUNC(tone_init);
FAKE_VOID_FUNC(tone_play, uint16_t, uint16_t);

static relay_t fake_relay;
static observer_t fake_observer;

uint8_t PORTC = 0x00; // Fake port for relay
#define PC7 7;        // Fake pin for relay

void setUp(void)
{
    RESET_FAKE(relay_create);
    RESET_FAKE(relay_destroy);
    RESET_FAKE(relay_on);
    RESET_FAKE(relay_off);
    RESET_FAKE(relay_is_on);
    RESET_FAKE(observer_create);
    RESET_FAKE(observer_destroy);
    RESET_FAKE(tone_init);
    RESET_FAKE(tone_play);

    fake_relay = (relay_t)0x1;
    fake_observer = (observer_t)0x2;

    relay_create_fake.return_val = fake_relay;
    observer_create_fake.return_val = fake_observer;
    relay_is_on_fake.return_val = 0;
}

void tearDown(void)
{
}

void test_pump_observer_create_success(void)
{
    observer_t obs = pump_observer_create();
    TEST_ASSERT_EQUAL_PTR(fake_observer, obs);
    TEST_ASSERT_EQUAL(1, observer_create_fake.call_count);
    TEST_ASSERT_EQUAL(1, relay_create_fake.call_count);
    TEST_ASSERT_EQUAL(1, tone_init_fake.call_count);
}

void test_pump_observer_create_fails_if_observer_create_fails(void)
{
    observer_create_fake.return_val = NULL;
    observer_t obs = pump_observer_create();

    TEST_ASSERT_NULL(obs);
    TEST_ASSERT_EQUAL(1, observer_create_fake.call_count);
    TEST_ASSERT_EQUAL(0, relay_create_fake.call_count);
    TEST_ASSERT_EQUAL(0, tone_init_fake.call_count);

    pump_observer_destroy(obs);
}

void test_pump_observer_create_fails_if_relay_create_fails(void)
{
    relay_create_fake.return_val = NULL;
    observer_t obs = pump_observer_create();

    TEST_ASSERT_NULL(obs);
    TEST_ASSERT_EQUAL(1, observer_create_fake.call_count);
    TEST_ASSERT_EQUAL(1, relay_create_fake.call_count);
    TEST_ASSERT_EQUAL(0, tone_init_fake.call_count);
    TEST_ASSERT_EQUAL(1, observer_destroy_fake.call_count);
    TEST_ASSERT_EQUAL_PTR(fake_observer, observer_destroy_fake.arg0_val);

    pump_observer_destroy(obs);
}

void test_pump_observer_destroy_calls_observer_destroy_and_relay_destroy(void)
{
    observer_t obs = pump_observer_create();
    pump_observer_destroy(obs);

    TEST_ASSERT_EQUAL(1, observer_destroy_fake.call_count);
    TEST_ASSERT_EQUAL_PTR(obs, observer_destroy_fake.arg0_val);
    TEST_ASSERT_EQUAL(1, relay_destroy_fake.call_count);
    TEST_ASSERT_EQUAL_PTR(fake_relay, relay_destroy_fake.arg0_val);
}

void test_pump_observer_destroy_calls_relay_destroy_when_observer_null(void)
{
    observer_t obs = pump_observer_create(); // Create to set up relay
    pump_observer_destroy(NULL);

    TEST_ASSERT_EQUAL(0, observer_destroy_fake.call_count);
    TEST_ASSERT_EQUAL(1, relay_destroy_fake.call_count);
    TEST_ASSERT_EQUAL_PTR(fake_relay, relay_destroy_fake.arg0_val);

    pump_observer_destroy(obs);
}

void test_pump_observer_destroy_does_nothing_if_null(void)
{
    relay_create_fake.return_val = NULL;
    pump_observer_destroy(NULL);

    TEST_ASSERT_EQUAL(0, observer_destroy_fake.call_count);
    TEST_ASSERT_EQUAL(0, relay_destroy_fake.call_count);
}

void test_pump_observer_callback_moisture_water_too_low_turns_off_pump_and_plays_warning(void)
{
    observer_callback_t cb = NULL;
    observer_t obs = pump_observer_create();
    cb = observer_create_fake.arg0_val;
    TEST_ASSERT_NOT_NULL(cb);

    // Set last_water_level low
    cb("water_level", "10", "mm");
    relay_is_on_fake.return_val = 1; // pump is on

    cb("moisture", "10", "%");

    TEST_ASSERT_EQUAL(1, relay_is_on_fake.call_count);
    TEST_ASSERT_EQUAL(1, relay_off_fake.call_count);
    TEST_ASSERT_EQUAL(1, tone_play_fake.call_count);

    pump_observer_destroy(obs);
}

void test_pump_observer_callback_moisture_soil_too_dry_turns_on_pump_and_plays_start_tones(void)
{
    observer_callback_t cb = NULL;
    observer_t obs = pump_observer_create();
    cb = observer_create_fake.arg0_val;
    TEST_ASSERT_NOT_NULL(cb);

    // Set last_water_level safe
    cb("water_level", "100", "mm");
    relay_is_on_fake.return_val = 0; // pump is off

    cb("moisture", "10", "%");

    TEST_ASSERT_EQUAL(1, relay_is_on_fake.call_count);
    TEST_ASSERT_EQUAL(1, relay_on_fake.call_count);
    TEST_ASSERT_EQUAL(2, tone_play_fake.call_count);

    pump_observer_destroy(obs);
}

void test_pump_observer_callback_moisture_soil_sufficient_turns_off_pump_and_plays_done_tones(void)
{
    observer_callback_t cb = NULL;
    observer_t obs = pump_observer_create();
    cb = observer_create_fake.arg0_val;
    TEST_ASSERT_NOT_NULL(cb);

    // Set last_water_level safe
    cb("water_level", "100", "mm");
    relay_is_on_fake.return_val = 1; // pump is on

    cb("moisture", "80", "%");

    TEST_ASSERT_EQUAL(1, relay_is_on_fake.call_count);
    TEST_ASSERT_EQUAL(1, relay_off_fake.call_count);
    TEST_ASSERT_EQUAL(4, tone_play_fake.call_count);

    pump_observer_destroy(obs);
}

void test_pump_observer_callback_ignores_null_args_or_relay(void)
{
    observer_callback_t cb = NULL;
    relay_create_fake.return_val = NULL;
    observer_t obs = pump_observer_create();
    cb = observer_create_fake.arg0_val;
    TEST_ASSERT_NOT_NULL(cb);

    cb(NULL, "10", "%");
    cb("moisture", NULL, "%");
    cb("moisture", "10", NULL);

    TEST_ASSERT_EQUAL(0, relay_is_on_fake.call_count);
    TEST_ASSERT_EQUAL(0, relay_off_fake.call_count);
    TEST_ASSERT_EQUAL(0, relay_on_fake.call_count);
    TEST_ASSERT_EQUAL(0, tone_play_fake.call_count);

    pump_observer_destroy(obs);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_pump_observer_create_success);
    RUN_TEST(test_pump_observer_create_fails_if_observer_create_fails);
    RUN_TEST(test_pump_observer_create_fails_if_relay_create_fails);
    RUN_TEST(test_pump_observer_destroy_calls_observer_destroy_and_relay_destroy);
    RUN_TEST(test_pump_observer_destroy_calls_relay_destroy_when_observer_null);
    RUN_TEST(test_pump_observer_destroy_does_nothing_if_null);
    RUN_TEST(test_pump_observer_callback_moisture_water_too_low_turns_off_pump_and_plays_warning);
    RUN_TEST(test_pump_observer_callback_moisture_soil_too_dry_turns_on_pump_and_plays_start_tones);
    RUN_TEST(test_pump_observer_callback_moisture_soil_sufficient_turns_off_pump_and_plays_done_tones);
    RUN_TEST(test_pump_observer_callback_ignores_null_args_or_relay);
    return UNITY_END();
}

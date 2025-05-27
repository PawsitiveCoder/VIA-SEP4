#include <unity.h>
#include <fff.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "relay.h"

DEFINE_FFF_GLOBALS;

static volatile uint8_t *fake_port_ptr;
static volatile uint8_t *fake_ddr_ptr;
static uint8_t fake_pin;

void setUp(void)
{
    static uint8_t fake_mem[2];

    // Simulate DDRx being one address before PORTx
    fake_ddr_ptr = &fake_mem[0];
    fake_port_ptr = &fake_mem[1];

    *fake_ddr_ptr = 0;
    *fake_port_ptr = 0;
    fake_pin = 0;
}

void tearDown(void)
{
}

void test_relay_create_sets_ddr_and_off(void)
{
    relay_t relay = relay_create(fake_port_ptr, fake_pin);

    TEST_ASSERT_NOT_NULL(relay);
    // Pin 2 should be set as output
    TEST_ASSERT_BITS_HIGH(1 << fake_pin, *fake_ddr_ptr);
    // Pin 2 should be off
    TEST_ASSERT_BITS_LOW(1 << fake_pin, *fake_port_ptr);

    relay_destroy(relay);
}

void test_relay_destroy_null_safe(void)
{
    relay_destroy(NULL);
}

void test_relay_destroy_turns_off_and_resets_ddr(void)
{
    relay_t relay = relay_create(fake_port_ptr, fake_pin);

    // Simulate relay on
    *fake_port_ptr |= (1 << fake_pin);
    TEST_ASSERT_BITS_HIGH(1 << fake_pin, *fake_port_ptr);

    relay_destroy(relay);
    // Pin should be off and DDR cleared
    TEST_ASSERT_BITS_LOW(1 << fake_pin, *fake_port_ptr);
    TEST_ASSERT_BITS_LOW(1 << fake_pin, *fake_ddr_ptr);
}

void test_relay_on_sets_port_bit(void)
{
    relay_t relay = relay_create(fake_port_ptr, fake_pin);

    relay_on(relay);
    TEST_ASSERT_BITS_HIGH(1 << fake_pin, *fake_port_ptr);

    relay_destroy(relay);
}

void test_relay_off_clears_port_bit(void)
{
    relay_t relay = relay_create(fake_port_ptr, fake_pin);

    // Simulate relay on
    *fake_port_ptr |= (1 << fake_pin);

    relay_off(relay);
    TEST_ASSERT_BITS_LOW(1 << fake_pin, *fake_port_ptr);

    relay_destroy(relay);
}

void test_relay_toggle_flips_port_bit(void)
{
    relay_t relay = relay_create(fake_port_ptr, fake_pin);

    // Initially off
    TEST_ASSERT_BITS_LOW(1 << fake_pin, *fake_port_ptr);

    relay_toggle(relay);
    TEST_ASSERT_BITS_HIGH(1 << fake_pin, *fake_port_ptr);

    relay_toggle(relay);
    TEST_ASSERT_BITS_LOW(1 << fake_pin, *fake_port_ptr);

    relay_destroy(relay);
}

void test_relay_is_on_returns_correct_state(void)
{
    relay_t relay = relay_create(fake_port_ptr, fake_pin);

    // Initially off
    TEST_ASSERT_EQUAL_UINT8(0, relay_is_on(relay));

    // Simulate relay on
    *fake_port_ptr |= (1 << fake_pin);
    TEST_ASSERT_EQUAL_UINT8(1, relay_is_on(relay));

    // Simulate relay off
    *fake_port_ptr &= ~(1 << fake_pin);
    TEST_ASSERT_EQUAL_UINT8(0, relay_is_on(relay));

    relay_destroy(relay);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_relay_create_sets_ddr_and_off);
    RUN_TEST(test_relay_destroy_null_safe);
    RUN_TEST(test_relay_destroy_turns_off_and_resets_ddr);
    RUN_TEST(test_relay_on_sets_port_bit);
    RUN_TEST(test_relay_off_clears_port_bit);
    RUN_TEST(test_relay_toggle_flips_port_bit);
    RUN_TEST(test_relay_is_on_returns_correct_state);
    return UNITY_END();
}

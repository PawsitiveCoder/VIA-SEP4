/**
 * @file relay.h
 * @brief Header file for relay driver.
 * This file provides functions to control a relay connected to a specific pin on a microcontroller.
 * It allows for creating, destroying, and toggling the state of the relay.
 */
#pragma once
#include <stdint.h>

/**
 * @brief Opaque type representing a relay.
 * This type is used to encapsulate the details of the relay implementation.
 */
typedef struct relay *relay_t;

/**
 * @brief Creates a relay instance.
 *
 * @param port Pointer to the port register (e.g., &PORTC).
 * @param pin Pin number within the port (e.g., PC7).
 * @return relay_t A pointer to the created relay instance, or NULL if memory allocation fails.
 */
relay_t relay_create(volatile uint8_t *port, uint8_t pin);

/**
 * @brief Destroys a relay instance.
 *
 * @param self Pointer to the relay instance to be destroyed.
 */
void relay_destroy(relay_t self);

/**
 * @brief Turns the relay on.
 *
 * @param self Pointer to the relay instance to turn on.
 */
void relay_on(relay_t self);

/**
 * @brief Turns the relay off.
 *
 * @param self Pointer to the relay instance to turn off.
 */
void relay_off(relay_t self);

/**
 * @brief Toggles the state of the relay.
 * If the relay is currently on, it will be turned off, and vice versa.
 *
 * @param self Pointer to the relay instance to toggle.
 */
void relay_toggle(relay_t self);

/**
 * @brief Checks if the relay is currently on.
 *
 * @param self Pointer to the relay instance to check.
 * @return 1 if the relay is on, 0 if it is off.
 */
uint8_t relay_is_on(relay_t self);

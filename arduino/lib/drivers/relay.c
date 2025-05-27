#include "includes.h"
#include "relay.h"

/// Implementation of the #relay_t type
typedef struct relay
{
    /// Pointer to the port register (PORTx)
    volatile uint8_t *port;
    /// Pointer to the data direction register (DDRx)
    volatile uint8_t *ddr;
    /// Pin number within the port (Pxx)
    uint8_t pin;
} relay;

relay_t relay_create(volatile uint8_t *port, uint8_t pin)
{
    relay_t _relay = calloc(1, sizeof(relay));

    if (_relay == NULL)
    {
        return NULL;
    }

    _relay->port = port;
    // DDRx is one register before PORTx
    _relay->ddr = port - 1;
    _relay->pin = pin;

    // Set pin as output
    *(_relay->ddr) |= (1 << _relay->pin);

    // Initialize relay to OFF state
    relay_off(_relay);

    return _relay;
}

void relay_destroy(relay_t self)
{
    if (self != NULL)
    {
        // Turn off the relay
        relay_off(self);

        // Reset pin to input
        *(self->ddr) &= ~(1 << self->pin);

        free(self);
    }
}

void relay_on(relay_t self)
{
    *(self->port) |= (1 << self->pin);
}

void relay_off(relay_t self)
{
    *(self->port) &= ~(1 << self->pin);
}

void relay_toggle(relay_t self)
{
    *(self->port) ^= (1 << self->pin);
}

uint8_t relay_is_on(relay_t self)
{
    return (*(self->port) & (1 << self->pin)) != 0;
}

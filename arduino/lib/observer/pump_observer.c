#include "includes.h"
#include "relay.h"
#include "observer.h"
#include "tone.h"

/// Minimum water level to turn on the pump safely (mm)
#define PUMP_MIN_WATER_LEVEL 30
/// Relay port for the pump
#define PUMP_PORT ((volatile uint8_t *)&PORTC)
/// Relay pin for the pump
#define PUMP_PIN PC7

/// Default soil moisture treshold (%)
static int moisture_threshold = 40;
static int last_water_level = 0;
static relay_t pump_relay = NULL;

static void pump_observer_callback(const char *sensor, const char *value, const char *unit)
{
    if (sensor == NULL || value == NULL || unit == NULL || pump_relay == NULL)
    {
        return;
    }

    if (strcmp(sensor, "water_level") == 0)
    {
        last_water_level = atoi(value);
    }
    else if (strcmp(sensor, "moisture") == 0)
    {
        int moisture_level = atoi(value);

        if (last_water_level <= PUMP_MIN_WATER_LEVEL)
        {
            if (relay_is_on(pump_relay))
            {
                // Turn off pump if water level is too low
                relay_off(pump_relay);
            }

            // Water too low warning tone
            tone_play(523, 1000);
        }
        else if (moisture_level < moisture_threshold)
        {
            // Water level is safe and soil is too dry
            if (!relay_is_on(pump_relay))
            {
                relay_on(pump_relay);
                // Watering started tone
                tone_play(1319, 80);
                tone_play(1568, 80);
            }
        }
        else if (relay_is_on(pump_relay))
        {
            // Water level is safe but moisture is sufficient
            relay_off(pump_relay);
            // Watering done tone
            tone_play(2093, 80);
            tone_play(2637, 80);
            tone_play(3136, 120);
            tone_play(4186, 180);
        }
    }
}

observer_t pump_observer_create(void)
{
    observer_t _pump_observer = observer_create(pump_observer_callback);
    if (_pump_observer == NULL)
    {
        return NULL;
    }

    pump_relay = relay_create(PUMP_PORT, PUMP_PIN);

    if (pump_relay == NULL)
    {
        observer_destroy(_pump_observer);
        return NULL;
    }

    tone_init();

    return _pump_observer;
}

void pump_observer_destroy(observer_t self)
{
    if (self != NULL)
    {
        observer_destroy(self);
    }
    if (pump_relay != NULL)
    {
        relay_destroy(pump_relay);
        pump_relay = NULL;
    }
}

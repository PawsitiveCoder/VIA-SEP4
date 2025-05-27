#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "soil.h"
#include "moisture_sensor.h"

/// Minimum senosor reading (in air)
static uint16_t dry_value = 534;
/// Maximum sensor reading (in water)
static uint16_t wet_value = 823;

typedef struct moisture_sensor
{
    float value;
    char unit[10];
} moisture_sensor;

moisture_sensor_t moisture_sensor_create(void)
{
    moisture_sensor_t _moisture_sensor = calloc(1, sizeof(moisture_sensor));

    if (_moisture_sensor == NULL)
    {
        return NULL;
    }

    _moisture_sensor->value = ERROR_VALUE;
    strcpy(_moisture_sensor->unit, MOISTURE_UNIT);

    soil_init(); // Initialize the driver

    return _moisture_sensor;
}

void moisture_sensor_destroy(moisture_sensor_t self)
{
    if (self != NULL)
    {
        free(self);
    }
}

void moisture_sensor_get_data(moisture_sensor_t self, float *value, char *unit)
{
    if (self == NULL || value == NULL || unit == NULL)
    {
        return;
    }

    *value = ERROR_VALUE;
    strcpy(unit, self->unit);

    uint16_t raw_value = soil_read();

    if (raw_value <= dry_value)
    {
        self->value = 0.0f;
    }
    else if (raw_value >= wet_value)
    {
        self->value = 100.0f;
    }
    else
    {
        // Linear interpolation between dry and wet values
        self->value = ((float)(raw_value - dry_value) / (wet_value - dry_value)) * 100.0f;
    }

    *value = self->value;
}

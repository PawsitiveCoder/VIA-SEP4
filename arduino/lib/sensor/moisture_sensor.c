#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "soil.h"
#include "moisture_sensor.h"

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

    self->value = (soil_read() * 100) / 1023; // Convert to percentage
    *value = self->value;
}

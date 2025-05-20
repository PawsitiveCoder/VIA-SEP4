#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "light.h"
#include "light_sensor.h"

typedef struct light_sensor
{
    float value;
    char unit[10];
} light_sensor;

light_sensor_t light_sensor_create(void)
{
    light_sensor_t _light_sensor = calloc(1, sizeof(light_sensor));

    if (_light_sensor == NULL)
    {
        return NULL;
    }

    _light_sensor->value = ERROR_VALUE;
    strcpy(_light_sensor->unit, LIGHT_UNIT);

    light_init(); // Initialize the driver

    return _light_sensor;
}

void light_sensor_destroy(light_sensor_t self)
{
    if (self != NULL)
    {
        free(self);
    }
}

void light_sensor_get_data(light_sensor_t self, float *value, char *unit)
{
    if (self == NULL || value == NULL || unit == NULL)
    {
        return;
    }
    
    *value = ERROR_VALUE;
    strcpy(unit, self->unit);

    self->value = 1023 - light_read();
    *value = self->value;
}
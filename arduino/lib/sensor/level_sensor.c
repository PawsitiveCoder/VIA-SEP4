#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "level_sensor.h"
#include "hc_sr04.h"

typedef struct level_sensor
{
    float value;
    char unit[10];
} level_sensor;

level_sensor_t level_sensor_create(void)
{
    level_sensor_t _level_sensor = calloc(1, sizeof(level_sensor));

    if (_level_sensor == NULL)
    {
        return NULL;
    }

    _level_sensor->value = ERROR_VALUE;
    strcpy(_level_sensor->unit, WATER_LEVEL_UNIT);

    hc_sr04_init(); // Initialize the driver

    return _level_sensor;
}

void level_sensor_destroy(level_sensor_t self)
{
    if (self != NULL)
    {
        free(self);
    }
}

void level_sensor_get_data(level_sensor_t self, float *value, char *unit)
{
    if (self == NULL || value == NULL || unit == NULL)
    {
        return;
    }

    *value = ERROR_VALUE;
    strcpy(unit, self->unit);

    uint16_t distance = hc_sr04_takeMeasurement(); // Distance in mm
    float water_level = WATER_LEVEL_MOUNT_HEIGHT - distance;

    if (water_level < 0 || water_level > WATER_LEVEL_MOUNT_HEIGHT)
    {

        water_level = ERROR_VALUE;
    }

    self->value = water_level;
    *value = self->value;
}

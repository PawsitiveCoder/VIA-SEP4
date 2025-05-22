#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pir.h"
#include "motion_sensor.h"

typedef struct motion_sensor
{
    float value; // 1.00 for motion detected, 0.00 for no motion
    char unit[10];
} motion_sensor;

static motion_sensor_t current_sensor = NULL;

static void motion_callback(void)
{
    if (current_sensor != NULL)
    {
        current_sensor->value = 1;
    }
}

motion_sensor_t motion_sensor_create(void)
{
    motion_sensor_t _motion_sensor = calloc(1, sizeof(motion_sensor));

    if (_motion_sensor == NULL)
    {
        return NULL;
    }

    _motion_sensor->value = ERROR_VALUE;
    strcpy(_motion_sensor->unit, MOTION_UNIT);

    current_sensor = _motion_sensor;

    pir_init(motion_callback); // Initialize the PIR driver with our callback

    return _motion_sensor;
}

void motion_sensor_destroy(motion_sensor_t self)
{
    if (self != NULL)
    {
        if (current_sensor == self)
        {
            current_sensor = NULL;
        }
        free(self);
    }
}

void motion_sensor_get_data(motion_sensor_t self, float *value, char *unit)
{
    if (self == NULL || value == NULL || unit == NULL)
    {
        return;
    }

    *value = ERROR_VALUE;
    strcpy(unit, self->unit);

    if (current_sensor != NULL)
    {
        self->value = current_sensor->value;
        *value = self->value;
        // Reset the value after reading
        current_sensor->value = 0.00;
    }
}

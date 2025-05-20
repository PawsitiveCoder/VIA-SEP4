#pragma once
#include "../../include/config.h"

typedef struct temperature_humidity_sensor *temperature_humidity_sensor_t;

temperature_humidity_sensor_t temperature_humidity_sensor_create();
void temperature_humidity_sensor_destroy(temperature_humidity_sensor_t self);

void temperature_humidity_sensor_get_data(
    temperature_humidity_sensor_t self,
    float *temperature_value,
    char *temperature_unit,
    float *humidity_value,
    char *humidity_unit
);

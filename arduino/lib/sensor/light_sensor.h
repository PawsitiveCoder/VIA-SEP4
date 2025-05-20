#pragma once
#include "../../include/config.h"

typedef struct light_sensor *light_sensor_t;

light_sensor_t light_sensor_create(void);
void light_sensor_destroy(light_sensor_t self);

void light_sensor_get_data(light_sensor_t self, float *value, char *unit);
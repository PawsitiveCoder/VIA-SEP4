#pragma once
#include "../../include/config.h"

typedef struct level_sensor *level_sensor_t;

level_sensor_t level_sensor_create(void);
void level_sensor_destroy(level_sensor_t self);

void level_sensor_get_data(level_sensor_t self, float *value, char *unit);

#pragma once
#include "../../include/config.h"

typedef struct motion_sensor *motion_sensor_t;
motion_sensor_t motion_sensor_create(void);
void motion_sensor_destroy(motion_sensor_t self);

void motion_sensor_get_data(motion_sensor_t self, float *value, char *unit);

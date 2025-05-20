#pragma once
#include "../../include/config.h"

typedef struct moisture_sensor *moisture_sensor_t;

moisture_sensor_t moisture_sensor_create(void);
void moisture_sensor_destroy(moisture_sensor_t self);

void moisture_sensor_get_data(moisture_sensor_t self, float *value, char *unit);

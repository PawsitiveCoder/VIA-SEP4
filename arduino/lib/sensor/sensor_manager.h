#pragma once
#include "observer.h"

typedef struct sensor_manager *sensor_manager_t;

sensor_manager_t sensor_manager_create(void);
void sensor_manager_destroy(sensor_manager_t self);

void sensor_manager_add_observer(sensor_manager_t self, observer_t observer);
void sensor_manager_read(sensor_manager_t self);

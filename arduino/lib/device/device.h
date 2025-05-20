#pragma once
#include "observer.h"

typedef struct device *device_t;

device_t device_create(const char *id, const char *version);
void device_destroy(device_t self);

void device_add_observer(device_t self, observer_t observer);
void device_process(device_t self);

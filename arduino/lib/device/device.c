#include <stdlib.h>
#include <string.h>
#include "device.h"
#include "sensor_manager.h"

typedef struct device
{
    char name[20];
    char version[10];
    sensor_manager_t sensor_manager;
} device;

device_t device_create(const char *name, const char *version)
{
    device_t _device = calloc(1, sizeof(device));

    if (_device == NULL)
    {
        return NULL;
    }

    strncpy(_device->name, name, sizeof(_device->name));
    strncpy(_device->version, version, sizeof(_device->version));
    _device->sensor_manager = sensor_manager_create();

    return _device;
}

void device_destroy(device_t self)
{
    if (self != NULL)
    {
        free(self->name);
        free(self->version);
        sensor_manager_destroy(self->sensor_manager);
        free(self);
    }
}

void device_add_observer(device_t self, observer_t observer)
{
    sensor_manager_add_observer(self->sensor_manager, observer);
}

void device_process(device_t self)
{
    sensor_manager_read(self->sensor_manager);
}

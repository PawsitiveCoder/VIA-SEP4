#include <stdlib.h>
#include <string.h>
#include "device.h"
#include "sensor_manager.h"
#include "periodic_task.h"

typedef struct device
{
    char name[20];
    char version[10];
    uint32_t processing_interval;
    uint8_t is_processing;
    sensor_manager_t sensor_manager;
} device;

static device_t current_device = NULL;

static void sensor_manager_read_task(void)
{
    if (current_device != NULL)
    {
        sensor_manager_read(current_device->sensor_manager);
    }
}

device_t device_create(const char *name, const char *version, uint32_t processing_interval)
{
    device_t _device = calloc(1, sizeof(device));

    if (_device == NULL)
    {
        return NULL;
    }

    strncpy(_device->name, name, sizeof(_device->name));
    strncpy(_device->version, version, sizeof(_device->version));
    _device->processing_interval = processing_interval;
    _device->is_processing = 0;
    _device->sensor_manager = sensor_manager_create();
    current_device = _device;

    return _device;
}

void device_destroy(device_t self)
{
    if (self != NULL)
    {
        if (current_device == self)
        {
            current_device = NULL;
        }

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
    if (self == NULL || self->is_processing)
    {
        return;
    }

    // Initialize an interrupt task to read sensor data at the specified interval
    periodic_task_init_a(sensor_manager_read_task, self->processing_interval);
}

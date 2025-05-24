#include <stdlib.h>
#include <stdio.h>
#include "subject.h"

#include "sensor_manager.h"

#include "temperature_humidity_sensor.h"
#include "moisture_sensor.h"
#include "light_sensor.h"
#include "motion_sensor.h"
#include "level_sensor.h"

#ifdef TEST_NATIVE
static const char *float_to_string(float value)
{
    static char string[32];
    snprintf(string, sizeof(string), "%.2f", value);
    return string;
}
#else
static const char *float_to_string(float value)
{
    static char string[32];
    dtostrf(value, 0, 2, string);
    return string;
}
#endif

typedef struct sensor_manager
{
    subject_t subject;
    temperature_humidity_sensor_t temperature_humidity_sensor;
    moisture_sensor_t moisture_sensor;
    light_sensor_t light_sensor;
    motion_sensor_t motion_sensor;
    level_sensor_t level_sensor;
} sensor_manager;

sensor_manager_t sensor_manager_create(void)
{
    sensor_manager_t _sensor_manager = calloc(1, sizeof(sensor_manager));

    if (_sensor_manager == NULL)
    {
        return NULL;
    }

    _sensor_manager->subject = subject_create();

    _sensor_manager->temperature_humidity_sensor = temperature_humidity_sensor_create();
    _sensor_manager->moisture_sensor = moisture_sensor_create();
    _sensor_manager->light_sensor = light_sensor_create();
    _sensor_manager->motion_sensor = motion_sensor_create();
    _sensor_manager->level_sensor = level_sensor_create();

    return _sensor_manager;
}

void sensor_manager_destroy(sensor_manager_t self)
{
    if (self != NULL)
    {
        subject_destroy(self->subject);
        temperature_humidity_sensor_destroy(self->temperature_humidity_sensor);
        moisture_sensor_destroy(self->moisture_sensor);
        light_sensor_destroy(self->light_sensor);
        motion_sensor_destroy(self->motion_sensor);
        level_sensor_destroy(self->level_sensor);

        free(self);
    }
}

void sensor_manager_add_observer(sensor_manager_t self, observer_t observer)
{
    subject_add_observer(self->subject, observer);
}

void sensor_manager_read(sensor_manager_t self)
{
    // Air temperature and humidity
    float temperature_value;
    char temperature_unit[10];
    float humidity_value;
    char humidity_unit[10];
    temperature_humidity_sensor_get_data(
        self->temperature_humidity_sensor,
        &temperature_value,
        temperature_unit,
        &humidity_value,
        humidity_unit);
    subject_notify_all(self->subject, "temperature", float_to_string(temperature_value), temperature_unit);
    subject_notify_all(self->subject, "humidity", float_to_string(humidity_value), humidity_unit);

    // Soil moisture
    float moisture_value;
    char moisture_unit[10];
    moisture_sensor_get_data(self->moisture_sensor, &moisture_value, moisture_unit);
    subject_notify_all(self->subject, "moisture", float_to_string(moisture_value), moisture_unit);

    // Light
    float light_value;
    char light_unit[10];
    light_sensor_get_data(self->light_sensor, &light_value, light_unit);
    subject_notify_all(self->subject, "light_level", float_to_string(light_value), light_unit);

    // Motion
    float motion_value;
    char motion_unit[10];
    motion_sensor_get_data(self->motion_sensor, &motion_value, motion_unit);
    subject_notify_all(self->subject, "motion", float_to_string(motion_value), motion_unit);

    // Water level
    float level_value;
    char level_unit[10];
    level_sensor_get_data(self->level_sensor, &level_value, level_unit);
    subject_notify_all(self->subject, "water_level", float_to_string(level_value), level_unit);
}

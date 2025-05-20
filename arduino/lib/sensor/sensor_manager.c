#include <stdlib.h>
#include "subject.h"

#include "sensor_manager.h"

#include "temperature_humidity_sensor.h"
#include "moisture_sensor.h"
#include "light_sensor.h"

typedef struct sensor_manager
{
    subject_t subject;
    temperature_humidity_sensor_t temperature_humidity_sensor;
    moisture_sensor_t moisture_sensor;
    light_sensor_t light_sensor;
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
    char temperature_value_string[10];
    char temperature_unit[10];
    float humidity_value;
    char humidity_value_string[10];
    char humidity_unit[10];
    temperature_humidity_sensor_get_data(
        self->temperature_humidity_sensor,
        &temperature_value,
        temperature_unit,
        &humidity_value,
        humidity_unit
    );
    dtostrf(temperature_value, 4, 2, temperature_value_string);
    dtostrf(humidity_value, 4, 2, humidity_value_string);
    subject_notify_all(self->subject, "temperature", temperature_value_string, temperature_unit);
    subject_notify_all(self->subject, "humidity", humidity_value_string, humidity_unit);

    // Soil moisture
    float moisture_value;
    char moisture_value_string[10];
    char moisture_unit[10];
    moisture_sensor_get_data(self->moisture_sensor, &moisture_value, moisture_unit);
    dtostrf(moisture_value, 4, 2, moisture_value_string);
    subject_notify_all(self->subject, "moisture", moisture_value_string, moisture_unit);

    // Light
    float light_value;
    char light_value_string[10];
    char light_unit[10];
    light_sensor_get_data(self->light_sensor, &light_value, light_unit);
    dtostrf(light_value, 4, 2, light_value_string);
    subject_notify_all(self->subject, "light_level", light_value_string, light_unit);
}
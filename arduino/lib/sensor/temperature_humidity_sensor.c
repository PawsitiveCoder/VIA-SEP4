#include <stdlib.h>
#include <string.h>
#include "dht11.h"
#include "temperature_humidity_sensor.h"

typedef struct temperature_humidity_sensor
{
    float temperature_value;
    char temperature_unit[10];
    float humidity_value;
    char humidity_unit[10];
} temperature_humidity_sensor;

temperature_humidity_sensor_t temperature_humidity_sensor_create()
{
    temperature_humidity_sensor_t _temperature_humidity = calloc(1, sizeof(temperature_humidity_sensor));

    if (_temperature_humidity == NULL)
    {
        return NULL;
    }

    _temperature_humidity->temperature_value = ERROR_VALUE;
    strcpy(_temperature_humidity->temperature_unit, TEMPERATURE_UNIT);
    
    _temperature_humidity->humidity_value = ERROR_VALUE;
    strcpy( _temperature_humidity->humidity_unit,  HUMIDITY_UNIT);

    dht11_init(); // Initialize the driver

    return _temperature_humidity;
}

void temperature_humidity_sensor_destroy(temperature_humidity_sensor_t self)
{
    if (self != NULL)
    {
        free(self);
    }
}

void temperature_humidity_sensor_get_data(
    temperature_humidity_sensor_t self,
    float *temperature_value,
    char *temperature_unit,
    float *humidity_value,
    char *humidity_unit
)
{
    if (self == NULL || temperature_value == NULL || temperature_unit == NULL || humidity_value == NULL || humidity_unit == NULL)
    {
        return;
    }
    
    *temperature_value = ERROR_VALUE;
    *humidity_value = ERROR_VALUE;
    strcpy(temperature_unit, self->temperature_unit);
    strcpy(humidity_unit, self->humidity_unit);

    uint8_t humidity_integer, humidity_decimal, temperature_integer, temperature_decimal;

    if (dht11_get(&humidity_integer, &humidity_decimal, &temperature_integer, &temperature_decimal) == DHT11_OK)
    {
        self->humidity_value = humidity_integer + (humidity_decimal / 100.0); // Convert to float
        self->temperature_value = temperature_integer + (temperature_decimal / 100.0); // Convert to float
 
        *temperature_value = self->temperature_value;
        *humidity_value = self->humidity_value;
    }    
}
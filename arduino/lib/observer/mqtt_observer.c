#include "mqtt_observer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/config.h"

static MQTT_Client_t client;

static void mqtt_observer_callback(const char *sensor, const char *value, const char *unit)
{
    char topic[64];
    char message[128];

    snprintf(topic, sizeof(topic), "%s/%s", DEVICE_ID, sensor);
    snprintf(message, sizeof(message), "{\"value\":\"%s\",\"unit\":\"%s\"}", value, unit);

    mqtt_publish(&client, topic, message, strlen(message));
}

observer_t mqtt_observer_create(MQTT_Client_t *mqtt_client)
{
    observer_t _mqtt_observer = observer_create(mqtt_observer_callback);

    if (_mqtt_observer == NULL) {
        return NULL;
    }

    client = *mqtt_client;

    return _mqtt_observer;
}

void mqtt_observer_destroy(observer_t self)
{
    if (self != NULL) {
        observer_destroy(self);
    }
}
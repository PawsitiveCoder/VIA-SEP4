#pragma once
#include "mqtt.h"
#include "observer.h"

observer_t mqtt_observer_create(MQTT_Client_t *mqtt_client);
void mqtt_observer_destroy(observer_t self);

#include "includes.h"
#include "wifi.h"
#include "mqtt.h"

// Global configuration
#include "config.h"

// Internal libraries
#include "console.h"
#include "device.h"
#include "console_observer.h"
#include "mqtt_observer.h"

void _wifi(void)
{
    wifi_init();
    console_log("Connecting to WiFi...\n");

    if (wifi_command_join_AP(WIFI_SSID, WIFI_PASSWORD) != WIFI_OK)
    {
        console_log("Failed to connect to WiFi.\n");
        while (1)
        {
            // Block on error
        };
    }

    console_log("Connected to WiFi.\n");
}

void _mqtt_callback(char *topic, char *message, uint16_t length)
{
    console_log("Received message on topic %s\n", topic);
}

MQTT_Client_t _mqtt(void)
{
    MQTT_Client_t client;
    mqtt_init(&client, MQTT_CLIENT_ID, MQTT_BROKER_IP, MQTT_BROKER_PORT, _mqtt_callback);
    console_log("Connecting to MQTT broker...\n");

    if (mqtt_connect(&client) != MQTT_OK)
    {
        console_log("Failed to connect to MQTT broker.\n");
        while (1)
        {
            // Block on error
        };
    }

    console_log("Connected to MQTT broker.\n");

    return client;
}

int main(void)
{
    console_init();
    _wifi();
    MQTT_Client_t mqtt_client = _mqtt();
    mqtt_subscribe(&mqtt_client, "greenhouse/commands");

    device_t device = device_create(DEVICE_ID, DEVICE_VERSION);

    observer_t console_observer = console_observer_create();
    observer_t mqtt_observer = mqtt_observer_create(&mqtt_client);

    device_add_observer(device, console_observer);
    device_add_observer(device, mqtt_observer);

    while (1)
    {
        mqtt_process(&mqtt_client);
        mqtt_publish(&mqtt_client, "greenhouse/status", "online", 6);
        device_process(device);
        _delay_ms(SENSOR_DELAY);
    }

    return 0;
}
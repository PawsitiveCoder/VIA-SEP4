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
#include "pump_observer.h"

static void setup_wifi(void)
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

static void setup_mqtt(MQTT_Client_t *client)
{
    mqtt_init(client, MQTT_CLIENT_ID, MQTT_BROKER_IP, MQTT_BROKER_PORT, NULL);
    console_log("Connecting to MQTT broker...\n");

    if (mqtt_connect(client) != MQTT_OK)
    {
        console_log("Failed to connect to MQTT broker.\n");
        while (1)
        {
            // Block on error
        };
    }

    console_log("Connected to MQTT broker.\n");
}

int main(void)
{
    console_init();
    setup_wifi();
    MQTT_Client_t mqtt_client;
    setup_mqtt(&mqtt_client);

    device_t device = device_create(DEVICE_ID, DEVICE_VERSION, DEVICE_PROCESSING_INTERVAL);

    observer_t console_observer = console_observer_create();
    observer_t mqtt_observer = mqtt_observer_create(&mqtt_client);
    observer_t pump_observer = pump_observer_create();

    device_add_observer(device, console_observer);
    device_add_observer(device, mqtt_observer);
    device_add_observer(device, pump_observer);

    device_process(device);

    while (1)
    {
        mqtt_process(&mqtt_client);
    }

    return 0;
}

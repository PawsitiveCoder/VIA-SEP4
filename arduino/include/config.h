// Global configuration
#pragma once

// Device configuration
#define DEVICE_ID "greenhouse"
#define DEVICE_VERSION "0.0.1"

// Wi-Fi configuration
#define WIFI_SSID "macbook"
#define WIFI_PASSWORD "qwerty123"

// MQTT configuration
#define MQTT_CLIENT_ID DEVICE_ID
#define MQTT_BROKER_IP "172.20.10.14"
#define MQTT_BROKER_PORT 1883

// Sensor configuration
#define SENSOR_DELAY 5000 // 5 seconds

// Common error value for all sensors
#define ERROR_VALUE -999.99

// Sensor units
#define TEMPERATURE_UNIT "˚C"
#define HUMIDITY_UNIT "%"
#define MOISTURE_UNIT "%"
#define LIGHT_UNIT "lux"
#define MOTION_UNIT "boolean"
#define WATER_LEVEL_UNIT "mm"

// The mount height of the water level sensor in mm
#define WATER_LEVEL_MOUNT_HEIGHT 100.00

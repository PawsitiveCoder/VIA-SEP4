/**
 * @file mqtt.h
 * @brief Barebones MQTT client implementation using ESP8266 WiFi.
 */
#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "wifi.h"

// MQTT packet types
#define MQTT_CONNECT 0x10
#define MQTT_CONNACK 0x20
#define MQTT_PUBLISH 0x30
#define MQTT_SUBSCRIBE 0x82
#define MQTT_SUBACK 0x90
#define MQTT_PINGREQ 0xC0
#define MQTT_PINGRESP 0xD0
#define MQTT_DISCONNECT 0xE0

// MQTT connection flags
#define MQTT_CLEAN_SESSION 0x02

// Return codes
typedef enum
{
    MQTT_OK,
    MQTT_ERROR_WIFI,
    MQTT_ERROR_CONNECT,
    MQTT_ERROR_SUBSCRIBE,
    MQTT_ERROR_PUBLISH
} MQTT_ERROR_t;

// Message callback function
typedef void (*MQTT_Callback_t)(char *topic, char *message, uint16_t length);

// MQTT client structure
typedef struct
{
    char *client_id;
    char *broker_ip;
    uint16_t broker_port;
    char *username;
    char *password;
    MQTT_Callback_t callback;
    char message_buffer[256];
} MQTT_Client_t;

/**
 * @brief Initialize the MQTT client
 *
 * @param client Pointer to client structure
 * @param client_id Client identifier string
 * @param broker_ip IP address of MQTT broker
 * @param broker_port Port of MQTT broker
 * @param callback Function to call when messages arrive
 * @return void
 */
void mqtt_init(MQTT_Client_t *client, char *client_id,
               char *broker_ip, uint16_t broker_port,
               MQTT_Callback_t callback);

/**
 * @brief Set authentication for MQTT connection
 *
 * @param client Pointer to client structure
 * @param username Username for authentication
 * @param password Password for authentication
 */
void mqtt_set_auth(MQTT_Client_t *client, char *username, char *password);

/**
 * @brief Connect to MQTT broker
 *
 * @param client Pointer to client structure
 * @return MQTT_ERROR_t Status code
 */
MQTT_ERROR_t mqtt_connect(MQTT_Client_t *client);

/**
 * @brief Publish a message to a topic
 *
 * @param client Pointer to client structure
 * @param topic Topic to publish to
 * @param message Message payload
 * @param length Length of message payload
 * @return MQTT_ERROR_t Status code
 */
MQTT_ERROR_t mqtt_publish(MQTT_Client_t *client, char *topic,
                          char *message, uint16_t length);

/**
 * @brief Subscribe to a topic
 *
 * @param client Pointer to client structure
 * @param topic Topic to subscribe to
 * @return MQTT_ERROR_t Status code
 */
MQTT_ERROR_t mqtt_subscribe(MQTT_Client_t *client, char *topic);

/**
 * @brief Disconnect from MQTT broker
 *
 * @param client Pointer to client structure
 * @return MQTT_ERROR_t Status code
 */
MQTT_ERROR_t mqtt_disconnect(MQTT_Client_t *client);

/**
 * @brief Process incoming messages (should be called periodically)
 *
 * @param client Pointer to client structure
 */
void mqtt_process(MQTT_Client_t *client);

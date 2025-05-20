#include "mqtt.h"
#include "wifi.h"
#include <string.h>
#include <stdlib.h>
#include "includes.h"

// Variable to keep track of message ID for SUBSCRIBE packets
static uint16_t message_id = 1;

// Flag to indicate that a message has been received
static volatile bool message_received = false;

// MQTT packet helper functions
static uint16_t mqtt_build_connect_packet(uint8_t *buffer, MQTT_Client_t *client)
{
    uint16_t length = 0;
    uint16_t client_id_len = strlen(client->client_id);
    uint16_t username_len = client->username ? strlen(client->username) : 0;
    uint16_t password_len = client->password ? strlen(client->password) : 0;

    // Fixed header
    buffer[length++] = MQTT_CONNECT; // Packet type

    // Remaining length (will be filled later)
    uint16_t remain_len_pos = length++;

    // Protocol name and level
    buffer[length++] = 0; // MSB
    buffer[length++] = 4; // LSB
    buffer[length++] = 'M';
    buffer[length++] = 'Q';
    buffer[length++] = 'T';
    buffer[length++] = 'T';
    buffer[length++] = 4; // Protocol level (MQTT 3.1.1)

    // Connect flags
    uint8_t connect_flags = MQTT_CLEAN_SESSION;
    if (client->username)
        connect_flags |= 0x80;
    if (client->password)
        connect_flags |= 0x40;
    buffer[length++] = connect_flags;

    // Keep alive (60 seconds)
    buffer[length++] = 0;
    buffer[length++] = 60;

    // Client ID
    buffer[length++] = (client_id_len >> 8) & 0xFF;
    buffer[length++] = client_id_len & 0xFF;
    memcpy(&buffer[length], client->client_id, client_id_len);
    length += client_id_len;

    // Username if present
    if (client->username)
    {
        buffer[length++] = (username_len >> 8) & 0xFF;
        buffer[length++] = username_len & 0xFF;
        memcpy(&buffer[length], client->username, username_len);
        length += username_len;
    }

    // Password if present
    if (client->password)
    {
        buffer[length++] = (password_len >> 8) & 0xFF;
        buffer[length++] = password_len & 0xFF;
        memcpy(&buffer[length], client->password, password_len);
        length += password_len;
    }

    // Fill in the remaining length
    buffer[remain_len_pos] = length - 2; // -2 for the fixed header

    return length;
}

static uint16_t mqtt_build_publish_packet(uint8_t *buffer, char *topic,
                                          char *message, uint16_t message_length)
{
    uint16_t length = 0;
    uint16_t topic_length = strlen(topic);

    // Fixed header
    buffer[length++] = MQTT_PUBLISH; // Packet type, QoS 0, no retain

    // Remaining length (will be filled later)
    uint16_t remain_len_pos = length++;

    // Topic
    buffer[length++] = (topic_length >> 8) & 0xFF;
    buffer[length++] = topic_length & 0xFF;
    memcpy(&buffer[length], topic, topic_length);
    length += topic_length;

    // Message
    memcpy(&buffer[length], message, message_length);
    length += message_length;

    // Fill in the remaining length
    buffer[remain_len_pos] = length - 2; // -2 for the fixed header

    return length;
}

static uint16_t mqtt_build_subscribe_packet(uint8_t *buffer, char *topic)
{
    uint16_t length = 0;
    uint16_t topic_length = strlen(topic);

    // Fixed header
    buffer[length++] = MQTT_SUBSCRIBE; // Packet type, QoS 1

    // Remaining length (will be filled later)
    uint16_t remain_len_pos = length++;

    // Packet identifier
    buffer[length++] = (message_id >> 8) & 0xFF;
    buffer[length++] = message_id & 0xFF;
    message_id++;

    // Topic
    buffer[length++] = (topic_length >> 8) & 0xFF;
    buffer[length++] = topic_length & 0xFF;
    memcpy(&buffer[length], topic, topic_length);
    length += topic_length;

    // QoS
    buffer[length++] = 0; // QoS 0

    // Fill in the remaining length
    buffer[remain_len_pos] = length - 2; // -2 for the fixed header

    return length;
}

static uint16_t mqtt_build_disconnect_packet(uint8_t *buffer)
{
    buffer[0] = MQTT_DISCONNECT;
    buffer[1] = 0; // Zero remaining length
    return 2;
}

// MQTT message received callback
static void mqtt_message_received()
{
    message_received = true;
}

// Parse MQTT packet
static void mqtt_parse_packet(MQTT_Client_t *client, char *buffer)
{
    uint8_t packet_type = buffer[0] & 0xF0;

    // Check if it's a PUBLISH packet
    if (packet_type == MQTT_PUBLISH)
    {
        // Parse PUBLISH packet
        uint16_t remaining_length = buffer[1];
        uint16_t topic_length = (buffer[2] << 8) | buffer[3];
        char topic[topic_length + 1];

        // Extract topic
        memcpy(topic, &buffer[4], topic_length);
        topic[topic_length] = '\0';

        // Extract payload
        uint16_t payload_length = remaining_length - topic_length - 2;
        char payload[payload_length + 1];
        memcpy(payload, &buffer[4 + topic_length], payload_length);
        payload[payload_length] = '\0';

        // Call user callback
        if (client->callback)
        {
            client->callback(topic, payload, payload_length);
        }
    }
}

// Public API implementation
void mqtt_init(MQTT_Client_t *client, char *client_id,
               char *broker_ip, uint16_t broker_port,
               MQTT_Callback_t callback)
{
    client->client_id = client_id;
    client->broker_ip = broker_ip;
    client->broker_port = broker_port;
    client->callback = callback;
    client->username = NULL;
    client->password = NULL;
}

void mqtt_set_auth(MQTT_Client_t *client, char *username, char *password)
{
    client->username = username;
    client->password = password;
}

MQTT_ERROR_t mqtt_connect(MQTT_Client_t *client)
{
    // Connect to broker using TCP
    WIFI_ERROR_MESSAGE_t wifi_error = wifi_command_create_TCP_connection(
        client->broker_ip,
        client->broker_port,
        mqtt_message_received,
        client->message_buffer);

    if (wifi_error != WIFI_OK)
    {
        return MQTT_ERROR_WIFI;
    }

    // Build MQTT CONNECT packet
    uint8_t connect_packet[128];
    uint16_t packet_length = mqtt_build_connect_packet(connect_packet, client);

    // Send CONNECT packet
    wifi_error = wifi_command_TCP_transmit(connect_packet, packet_length);
    if (wifi_error != WIFI_OK)
    {
        return MQTT_ERROR_CONNECT;
    }

    // Wait for CONNACK
    uint16_t timeout = 500; // 5 seconds
    while (!message_received && timeout > 0)
    {
        _delay_ms(10);
        timeout--;
    }

    if (timeout == 0)
    {
        return MQTT_ERROR_CONNECT;
    }

    message_received = false;

    // Check for connection success (very basic check)
    if (client->message_buffer[0] == MQTT_CONNACK && client->message_buffer[3] == 0)
    {
        return MQTT_OK;
    }
    else
    {
        return MQTT_ERROR_CONNECT;
    }
}

MQTT_ERROR_t mqtt_publish(MQTT_Client_t *client, char *topic,
                          char *message, uint16_t length)
{
    // Build MQTT PUBLISH packet
    uint8_t publish_packet[256];
    uint16_t packet_length = mqtt_build_publish_packet(publish_packet, topic, message, length);

    // Send PUBLISH packet
    WIFI_ERROR_MESSAGE_t wifi_error = wifi_command_TCP_transmit(publish_packet, packet_length);
    if (wifi_error != WIFI_OK)
    {
        return MQTT_ERROR_PUBLISH;
    }

    return MQTT_OK;
}

MQTT_ERROR_t mqtt_subscribe(MQTT_Client_t *client, char *topic)
{
    // Build MQTT SUBSCRIBE packet
    uint8_t subscribe_packet[128];
    uint16_t packet_length = mqtt_build_subscribe_packet(subscribe_packet, topic);

    // Send SUBSCRIBE packet
    WIFI_ERROR_MESSAGE_t wifi_error = wifi_command_TCP_transmit(subscribe_packet, packet_length);
    if (wifi_error != WIFI_OK)
    {
        return MQTT_ERROR_SUBSCRIBE;
    }

    // Wait for SUBACK
    uint16_t timeout = 500; // 5 seconds
    while (!message_received && timeout > 0)
    {
        _delay_ms(10);
        timeout--;
    }

    if (timeout == 0)
    {
        return MQTT_ERROR_SUBSCRIBE;
    }

    message_received = false;
    return MQTT_OK;
}

MQTT_ERROR_t mqtt_disconnect(MQTT_Client_t *client)
{
    // Build MQTT DISCONNECT packet
    uint8_t disconnect_packet[2];
    uint16_t packet_length = mqtt_build_disconnect_packet(disconnect_packet);

    // Send DISCONNECT packet
    wifi_command_TCP_transmit(disconnect_packet, packet_length);

    // Close TCP connection
    wifi_command_close_TCP_connection();

    return MQTT_OK;
}

void mqtt_process(MQTT_Client_t *client)
{
    if (message_received)
    {
        mqtt_parse_packet(client, client->message_buffer);
        message_received = false;
    }
}

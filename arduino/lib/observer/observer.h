#pragma once

typedef struct observer *observer_t;
typedef void (*observer_callback_t)(const char *topic, const char *value1, const char *value2);

observer_t observer_create(observer_callback_t callback);
void observer_destroy(observer_t self);

void observer_notify(observer_t self, const char *topic, const char *value1, const char *value2);

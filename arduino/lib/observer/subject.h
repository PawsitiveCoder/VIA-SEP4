#pragma once
#include "observer.h"

#define MAX_OBSERVERS 10

typedef struct subject *subject_t;

subject_t subject_create(void);
void subject_destroy(subject_t self);

void subject_add_observer(subject_t self, observer_t observer);
void subject_remove_observer(subject_t self, observer_t observer);
void subject_notify_all(subject_t self, const char *topic, const char *value1, const char *value2);

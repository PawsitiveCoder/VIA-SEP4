#include <stdlib.h>
#include <stdint.h>
#include "subject.h"

typedef struct subject {
    observer_t observers[MAX_OBSERVERS];
    uint8_t observers_count;
} subject;

subject_t subject_create(void) {
    subject_t _subject = calloc(1, sizeof(subject));

    if (_subject == NULL) {
        return NULL;
    }

    _subject->observers_count = 0;

    return _subject;
}

void subject_destroy(subject_t self) {
    if (self != NULL) {
        for (uint8_t i = 0; i < self->observers_count; i++) {
            observer_destroy(self->observers[i]);
        }
        free(self);
    }
}

void subject_add_observer(subject_t self, observer_t observer)
{
    if (self == NULL || observer == NULL) {
        return;
    }

    for (uint8_t i = 0; i < self->observers_count; i++) {
        if (self->observers[i] == observer) {
            return; // Already subscribed
        }
    }

    if (self->observers_count < MAX_OBSERVERS) {
        self->observers[self->observers_count++] = observer;
    }
}

void subject_remove_observer(subject_t self, observer_t observer)
{
    if (self == NULL || observer == NULL) {
        return;
    }

    for (uint8_t i = 0; i < self->observers_count; i++) {
        if (self->observers[i] == observer) {
            // Shift remaining observers down
            for (uint8_t j = i; j < self->observers_count - 1; j++) {
                self->observers[j] = self->observers[j + 1];
            }
            self->observers_count--;
            return;
        }
    }
}

void subject_notify_all(subject_t self, const char *topic, const char *value1, const char *value2)
{
    if (self == NULL) {
        return;
    }

    for (uint8_t i = 0; i < self->observers_count; i++) {
        observer_notify(self->observers[i], topic, value1, value2);
    }
}

#include <stdlib.h>
#include "observer.h"

typedef struct observer
{
    observer_callback_t callback;
} observer;

observer_t observer_create(observer_callback_t callback)
{
    observer_t _observer = calloc(1, sizeof(observer));

    if (_observer == NULL)
    {
        return NULL;
    }

    _observer->callback = callback;

    return _observer;
}

void observer_destroy(observer_t self)
{
    if (self != NULL)
    {
        free(self);
    }
}

void observer_notify(observer_t self, const char *topic, const char *value1, const char *value2)
{
    if (self != NULL && self->callback != NULL)
    {
        self->callback(topic, value1, value2);
    }
}

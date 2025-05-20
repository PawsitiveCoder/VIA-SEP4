#include <stdlib.h>
#include "console.h"
#include "console_observer.h"

static void console_observer_callback(const char *sensor, const char *value, const char *unit)
{
    console_log("Sensor: %s, Value: %s, Unit: %s\n", sensor, value, unit);
}

observer_t console_observer_create(void)
{
    observer_t _console_observer = observer_create(console_observer_callback);

    if (_console_observer == NULL) {
        return NULL;
    }

    console_init();

    return _console_observer;
}

void console_observer_destroy(observer_t self)
{
    if (self != NULL) {
        observer_destroy(self);
    }
}

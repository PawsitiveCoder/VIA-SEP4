#pragma once
#include "observer.h"

observer_t console_observer_create(void);
void console_observer_destroy(observer_t self);

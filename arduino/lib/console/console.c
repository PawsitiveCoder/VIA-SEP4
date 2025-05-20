#include <stdio.h>
#include <stdarg.h>
#include "uart.h"

#include "console.h"

#define BUILT_IN_CONSOLE USART_0
#define BUILT_IN_CONSOLE_BAUD 9600

static int initialized = 0;

void console_init(void)
{
    if (initialized)
    {
        return;
    }

    uart_init(BUILT_IN_CONSOLE, BUILT_IN_CONSOLE_BAUD, NULL);

    initialized = 1;
}

void console_log(const char *format, ...)
{
    char buffer[128];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    uart_send_string_blocking(BUILT_IN_CONSOLE, buffer);
}
#pragma once

#define BUILT_IN_CONSOLE USART_0
#define BUILT_IN_CONSOLE_BAUD 9600

void console_init(void);
void console_log(const char *format, ...);

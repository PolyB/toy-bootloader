#ifndef PRINTF_H
#define PRINTF_H

#include <stdarg.h>

int printf(const char *fmt, ...);
int sprintf(char *buf, const char *fmt, ...);
int vsprintf(char *buf, const char *fmt, va_list args);

#endif

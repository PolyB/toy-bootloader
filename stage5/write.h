#ifndef WRITE_H
#define WRITE_H

#include <stdint.h>
#include <stdarg.h>

void puts(char* str);

int vsprintf(char *buf, const char *fmt, va_list args);
int sprintf(char *buf, const char *fmt, ...);
int printf(const char *fmt, ...);

#endif

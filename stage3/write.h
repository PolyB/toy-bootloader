#ifndef WRITE_H
#define WRITE_H

#include <stddef.h>
#include <stdint.h>

int write(const char* buf, size_t count);
int puts(const char* buf);

size_t strnlen(const char *s, size_t maxlen);


#endif

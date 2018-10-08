#ifndef OUTIN_H
#define OUTIN_H

#include <stdint.h>

void do_outb(uint16_t port, uint8_t val);
uint8_t do_inb(uint16_t port);

#endif

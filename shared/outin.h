#ifndef SHARED_OUTIN_H
#define SHARED_OUTIN_H

#include <stdint.h>

static inline void do_outb(uint16_t port, uint8_t val)
{
	__asm__ volatile("outb %%al, %%dx" : : "d"(port), "a"(val));
}

static inline uint8_t do_inb(uint16_t port)
{
		unsigned char ret;
		__asm__ volatile("inb %%dx, %%al" : "=a"(ret) : "d"(port));
		return ret;
}

#endif

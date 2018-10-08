#include "outin.h"

void do_outb(uint16_t port, uint8_t val)
{
	__asm__ volatile("out %%al,%%dx" : : "d"(port), "a"(val));
}

uint8_t do_inb(uint16_t port)
{
		unsigned char ret;
		__asm__ volatile("inb %%dx,%%al" : "=a"(ret) : "d"(port));
		return ret;
}

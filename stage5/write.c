#include <stddef.h>
#include <stdint.h>

#include "outin.h"

void puts(const char* buf)
{
  const unsigned int com1 = 0x3F8;

  while (*buf)
  {
    char c = *buf++;
    if (c == '\n')
      do_outb(com1, '\r');
    do_outb(com1, c);
  }
  // unsigned char lsr;
  // lsr = do_inb(com1 + 5);
  // if (lsr & 0xE) -> error
}

size_t strnlen(const char *s, size_t maxlen)
{
	size_t i = 0;
	for (; i < maxlen; ++i)
		if (!s[i])
			return i;
	return i;
}

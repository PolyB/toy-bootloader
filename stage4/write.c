#include "shared/outin.h"
#include "write.h"

static inline void setbr(unsigned int com1)
{
  const unsigned char brateh = 0x00; // 38400bps
  const unsigned char bratel = 0x03; // 38400bps

  unsigned char lcr = do_inb(com1 + 3);
  do_outb(com1 + 3, lcr | 0x80); // Set DivisorLatchAccessBit
  do_outb(com1 + 0, bratel);
  do_outb(com1 + 1, brateh);
  do_outb(com1 + 3, (lcr & (~(0x80 | 0x38)))); //Set Noparity
}

static uint8_t is_init = 0;

int write(const char* buf, size_t count)
{
  const unsigned int com1 = 0x3F8;
  if (!is_init)
  {
    is_init = 1;
    setbr(com1);
  }

  for (size_t i = 0; i < count; i++)
  {
    char c = buf[i];
    if (c == '\n')
      do_outb(com1, '\r');
    do_outb(com1, c);
  }
  unsigned char lsr;
  lsr = do_inb(com1 + 5);
  if (lsr & 0xE)
    return -1;
  return count;
}

int puts(const char *s)
{
  const unsigned int com1 = 0x3F8;
  if (!is_init)
  {
    is_init = 1;
    setbr(com1);
  }
  while(*s)
  {
    char c = *s;
    if (c == '\n')
      do_outb(com1, '\r');
    do_outb(com1, c);
    s++;
  }

  unsigned char lsr;
  lsr = do_inb(com1 + 5);
  if (lsr & 0xE)
    return -1;
  return 1;
}

size_t strnlen(const char *s, size_t maxlen)
{
	size_t i = 0;
	for (; i < maxlen; ++i)
		if (!s[i])
			return i;
	return i;
}

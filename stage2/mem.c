#include "init.h"
#include "print.h"
#include "printf.h"
#include "shared/mem.h"

#define SMAP ('S' << 24 | 'M' << 16 | 'A' << 8 | 'P')


static void print_mem_sec(const struct mem_sec *m)
{
  printf("[base:0x%lp, length:0x%lx, type:%d]\n", m->base, m->length, m->type);
}

void set_memory_size(void)
{
  struct mem_sec buf;
  uint32_t a;
  uint32_t b;
  uint32_t c;
  uint8_t carry;

  // int15h, AX=E820
  __asm__ volatile ("int $0x15; setc %%dl"
                               :"=a"(a), 
                                "=b"(b), 
                                "=c"(c), 
                                "=d"(carry) 
                               : "a"(0xE820),
                                 "b"(0),
                                 "d"(SMAP),
                                 "c"(sizeof(buf)),
                                 "S"(0),
                                 "D"(&buf)
                                 : "memory");
  if (carry || a != SMAP)
  {
    printf("| error, carry=%d && a=%x\n", carry, a);
    printf("\\ buf = %p\n", buf);
  }
  else
  {
    print_mem_sec(&buf);
    printf("%cTODO:finish memory mapping%c\n", COLOR(red), COLOR(dark_gray));
  }
}

INIT(set_memory_size);

#include "init.h"
#include "idt.h"
#include "printf.h"
#include "write.h"

extern char _stage_3_start; // linker var

static void hexdump(int count, const uint8_t *ptr)
{
  for (int i = 0; i < count; i++)
  {
    if (!(i % 10))
      printf("\n");
    char h = ptr[i] >> 4;
    char l = ptr[i] & 0xF;
    printf("%c%c ", 
        h > 0x9 ? h - 0xA + 'A': h + '0',
        l > 0x9 ? l - 0xA + 'A': l + '0');
  }
  printf("\n");
}

static void handle_int3(struct int_infos *r)
{
  printf("*GOT DEBUG INTERRUPTION\n");
  printf("* regs: \n* edi:0x%x, esi:0x%x, edx:0x%x, ecx:0x%x, eax:0x%x, ip:0x%x\n",
      r->edi, r->esi, r->edx, r->ecx, r->eax, r->ip);
  printf("*stage3 local address : 0x%x \n", r->ip - (uint32_t)&_stage_3_start);
  printf("*next code: \n");
  hexdump(40, (void *)r->ip);
  printf("*END OF DEBUG\n");
}

void breakpoint_instr(void)
{
  set_interrupt(3, handle_int3);
}
INIT(breakpoint_instr);

#include <stddef.h>
#include <stdint.h>

#include "idt.h"
#include "init.h"
#include "printf.h"
#include "shared/debug.h"

extern struct init _stage3_init_start;
extern struct init _stage3_init_end;
extern uint8_t _stage_3_bss_start;
extern uint8_t _stage_3_bss_end;

void __attribute__((noreturn)) load_protected(void);

extern uint32_t stage_4_start; // defined in stage1/boot.c TODO : remove me

static void memreset(uint8_t *s, uint8_t *e)
{
  while (s != e)
    *s++ = 0;
}

void __attribute__((noreturn)) stage3_load(void) 
{
  printf("*********************\n\n");
  printf("stage3 loadled (now in protected)\n");
  printf("reseting bss\n");
  memreset(&_stage_3_bss_start, &_stage_3_bss_end);
  printf("executing stage3 :\n\n");
  unsigned int task_count = (&_stage3_init_end - &_stage3_init_start);
  for (struct init *f = &_stage3_init_start; f < &_stage3_init_end; f++)
  {
    unsigned int current_task = 1 + (f - &_stage3_init_start);
    printf("executing `%s` in `%s` (%u of %u)\n",
        f->name, f->file, current_task, task_count);
    f->fun();
  }
  printf("stage 3 finished\n");
  printf("jumping in load_protected\n");
  load_protected();
}

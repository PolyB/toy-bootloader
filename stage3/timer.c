#include "init.h"
#include "idt.h"
#include "shared/outin.h"

#define TIMER_INT 32

#define PIT_A 0x40
#define PIT_B 0x41
#define PIT_C 0x42
#define PIT_CONTROL 0x43

uint32_t timer = 0;

static void handle_timer_interrupt(struct int_infos* regs)
{
  printf("poc\n");
  if (timer)
    timer--;
}

static void init_timer(int hz)
{
  int div = 1193180 / hz;
  do_outb(PIT_CONTROL, 0x36); // set
  do_outb(PIT_A, div & 0xFF);
  do_outb(PIT_A, (div >> 8 )& 0xFF);
}

void sleep(uint32_t i)
{
  timer = i + 1;
  while (timer)
  {
    // TODO : hlt ?
  }
}
void sleep_exit(void)
{
  timer = 0;
}

void setup_timer(void)
{
  set_interrupt(TIMER_INT, handle_timer_interrupt);
  init_timer(10); // 1hz
  irq_unmask(0);
}

INIT(setup_timer);

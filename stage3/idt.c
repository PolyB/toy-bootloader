#include "idt.h"
#include "init.h"
#include "shared/outin.h"
#include "printf.h"
#include "shared/debug.h"
#include "shared/gdt.h"
#include "shared/idt.h"
#include "shared/idt.h"
#include "shared/pic.h"


struct idt_r {
	uint16_t table_limit : 16;
	struct idt_event* base_address;
} __attribute__((packed));


static void idt_set(uint8_t event_n, void* offset, struct seg_register seg_selector)
{
  int is_interrupt = IS_INTERRUPT(event_n);
  struct idt_event ev = {
    .offset0 = ((uint32_t)offset) & 0x0000FFFF,
    .offset1 = (((uint32_t)offset) & 0xFFFF0000) >> 16,
    .seg_selector = seg_selector,
    .p = 1,
    .idt_flag = is_interrupt ? 0x70 : 0x78, // size of gate = 32
    .dpl = 0,
  };
  g_idt[event_n] = ev;
}

void idt(void)
{
  debugf("idt pos : %p", g_idt);
#define X(N, err, info)              \
  idt_set(N, idt_entry_32_##N, seg_cs); \
  idt_handlers_32[N] = interrupt_panic;
  X_IDT_ENTRIES
#undef X

  struct idt_r idtr = {.table_limit = sizeof (g_idt) - 1,
                       .base_address = g_idt};
  __asm__ volatile("lidt %0"::"m"(idtr):"memory");
  printf("interruptions enabled\n");

}

void interrupt_panic(struct int_infos* r)
{
  printf("***PANIC***\n");
  printf("interruption %d\n", r->int_nbr);
  switch(r->int_nbr)
  {
#define XERRORCODE printf("error_code : 0x%x\n", r->ret_code);
#define XNOERRORCODE
#define X(N, ERR, INFO)                                     \
    case N:                                                 \
      printf("interruption informations : \"" INFO "\"\n"); \
      ERR                                                   \
      break;
    X_IDT_ENTRIES
#undef X
  }
  printf("edi:0x%x, esi:0x%x, edx:0x%x, ecx:0x%x, eax:0x%x, ip:0x%x\n",
      r->edi, r->esi, r->edx, r->ecx, r->eax, r->ip);
  for(;;);
}
void interrupt_none(struct int_infos *regs)
{
  (void)regs;
}

void irq_mask(uint8_t line)
{
  uint8_t port;
  if (line > 8)
  {
    port = PIC_MASTER_B;
  }
  else
  {
    port = PIC_SLAVE_B;
    line -= 8;
  }
  uint8_t curr = do_inb(port);
  do_outb(port, curr | (1 << line));
}
void irq_unmask(uint8_t line)
{
  uint8_t port;
  if (line > 8)
  {
    port = PIC_MASTER_B;
  }
  else
  {
    port = PIC_SLAVE_B;
    line -= 8;
  }
  uint8_t curr = do_inb(port);
  do_outb(port, curr & ~(1 << line));
}

void clear_interrupt(uint8_t n) { set_interrupt(n,interrupt_panic); }

void init_8259(void)
{
  /* ICW1 */
  do_outb(PIC_MASTER_A , 0x11);
	do_outb(PIC_SLAVE_A , 0x11);
  /* ICW2 */
  do_outb(PIC_MASTER_B, 0x20);
  do_outb(PIC_SLAVE_B, 0x28);
  /* ICW3 */
  do_outb(PIC_MASTER_B, 0x04);
  do_outb(PIC_SLAVE_B, 0x02);
  /* ICW4 */
  do_outb(PIC_MASTER_B, 0x01);
  do_outb(PIC_SLAVE_B, 0x01);
  /* OCW1 */
  do_outb(PIC_MASTER_B, 0x01);
  do_outb(PIC_SLAVE_B, 0x01);
  __asm__ volatile ("sti");
}

INIT(idt)
INIT(init_8259)

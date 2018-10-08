#include <stdint.h>
#include "init.h"
#include "misc.h"
#include "print.h"
#include "printf.h"
#include "shared/gdt.h"



struct gdtr
{
	uint16_t table_limit;
	struct gdt_seg_descriptor* base_address;
} __attribute__((packed));





#define GDT_TYPE_ACCESSED 0x1

#define GDT_TYPE_C 0x8
#define GDT_TYPE_C_CONFORMING 0x4
#define GDT_TYPE_C_READ 0x2

#define GDT_TYPE_D 0x0
#define GDT_TYPE_D_WRITE 0x2
#define GDT_TYPE_D_EXPEND 0x4

#define GDT_TYPE_FLAG 0x8




static void gdt_segment_create(struct gdt_seg_descriptor* gdt, uint32_t base,
															 uint32_t limit, uint8_t priv_level)
{
  if (limit > 0xFFFF)
  {
    limit >>= 12; // 4kb granularity
    gdt->granularity = 1; //multiples of 4kbyte
  }
  else
  {
    gdt->granularity = 0; //multiples of 1B
  }
	gdt->seg_limit_1 = limit;
	gdt->seg_limit_2 = limit >> 16;
	gdt->base_address_1 = base;
	gdt->base_address_2 = base >> 16;
	gdt->base_address_3 = base >> (16 + 8);

	gdt->_true = 1;//code/data
	gdt->privilelge_level = priv_level;
	gdt->segment_present = 1;
	gdt->available_system_software = 0;
	gdt->mode_64_bit = 0;
	gdt->op_size = 1; //32-bit segment
  gdt->readable_writable = 1;

}

void gdt_setup(void)
{
  printf("| starting gdt setup\n");
  struct gdtr gdtr = {.table_limit = sizeof(gdt) - 1, .base_address = gdt};

  gdt_segment_create(&gdt[1], 0, 0xFFFFFFFF, 0);
  gdt[1].code_sel = 1;

  gdt_segment_create(&gdt[2], 0, 0xFFFFFFFF, 0);
  gdt[2].code_sel = 0;

  gdt_segment_create(&gdt[3], 0, 0xFFFFFFFF, 0);
  gdt[3].code_sel = 1;
  gdt[3].mode_64_bit = 1; gdt[3].op_size = 0;

  struct seg_register cs = { .rpl = 0, .ti = 0, .index = 1};
  struct seg_register ds = { .rpl = 0, .ti = 0, .index = 2};
	__asm__ volatile("lgdt %0 \n"
			:
			: "m"(gdtr)
			: "memory");
  seg_ss = ds;
  seg_es = ds;
  seg_fs = ds;
  seg_gs = ds;
  seg_ds = ds;
  seg_cs = cs;
}

INIT(gdt_setup);

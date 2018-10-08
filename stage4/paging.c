#include "shared/paging.h"

#include <stddef.h>

#include "shared/sys_regs.h"
#include "shared/gdt.h"


struct gdtr
{
	uint16_t table_limit;
	struct gdt_seg_descriptor* base_address;
} __attribute__((packed));

pg_pml4 pml4[512] __attribute__((aligned(0x1000)));
pg_pdp pdp[512] __attribute__((aligned(0x1000)));
pg_pd pd[512] __attribute__((aligned(0x1000)));
pg_pt pt[512] __attribute__((aligned(0x1000)));

static inline void set_addr(struct pg_base *pg, void* addr)
{
  *(uint64_t*)pg = (uint64_t)(size_t)addr;
}

void setup_paging(void)
{
  set_addr(&pml4[0], &pdp[0]);
  pml4[0].present = 1;
  pml4[0].writable = 1;

  set_addr(&pdp[0], &pd[0]);
  pdp[0].present = 1;
  pdp[0].writable = 1;

  for (int i = 0; i < 512; i++)
  {
    void* addr = (void*)(0x200000 /* 2Mib*/ * i);
    set_addr(&pd[i], addr);
    pd[i].present = 1;
    pd[i].writable = 1;
    pd[i].huge_null = 1;
  }

  SET_REG(cr3, pml4)
  SET_BIT(cr4, 5); // PAE-flag

  uint32_t msr;
  __asm__ volatile ("rdmsr":"=a"(msr) : "c"(0xC0000080)); // read msr
  __asm__ volatile ("wrmsr"::"a"(msr | 1 << 8)); // set msr

  // enable paging;
  SET_BIT(cr0, 31);
  struct seg_register cs = { .rpl = 0, .ti = 0, .index = 3};
  struct seg_register ds = { .rpl = 0, .ti = 0, .index = 2};
  seg_es = ds;
  seg_fs = ds;
  seg_gs = ds;
  seg_ds = ds;
  seg_ss = ds;
  seg_cs = cs;

}

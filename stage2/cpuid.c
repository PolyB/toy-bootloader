#include <stdint.h>

#include "init.h"
#include "printf.h"
#include "print.h"
#include "shared/cpuid.h"

struct regs
{
  uint32_t eax;
  uint32_t ebx;
  uint32_t ecx;
  uint32_t edx;
};

static struct regs do_cpuid(uint32_t num)
{
  struct regs regs;
  __asm__ volatile ("cpuid" : "=a"(regs.eax),
                              "=b"(regs.ebx),
                              "=c"(regs.ecx),
                              "=d"(regs.edx)
                              : "a"(num));
  return regs;
}

static void print_state(const char *name, int enabled)
{
  printf(" %s:%c%c%c", name, 
                       enabled ? COLOR(green) : COLOR(red),
                       enabled ? 'Y' : 'N',
                       COLOR(light_gray));
}

void cpuid_1(void)
{
  struct regs r = do_cpuid(1);
  clflush_cl = (r.ebx >> 8) & 0xF;
  loc_apic_id = (r.ebx >> 24) & 0xF;
  proc_features = *(struct pfeatures*)&r.ecx; // strict aliasing, undefined behavior, but easier like this 
  printf("| CLFLUSH instruction cache line size : %d\n", clflush_cl);
  printf("| local APIC ID : %d\n", loc_apic_id);
  printf("| Detected CPU features : \n");
#define X(f) print_state(#f, proc_features.f);
  XPFEATURES1
#undef X
    printf("\n");
}

void cpuid_0x80000000(void)
{
  struct regs r = do_cpuid(0x80000000);
  max_cpuid = r.eax;
  printf("| max cpuid : %x\n", max_cpuid);
}

void cpuid_0x80000001(void)
{
  if (max_cpuid < 0x80000001)
  {
    printf("| not allowed by cpuid\n");
    return;
  }
  struct regs r = do_cpuid(0x80000001);
  cpuid_infos_extra_1c =*(struct cpuid_0x80000001_ecx*)&r.ecx; // strict aliasing, undefined behavior, but easier like this
  cpuid_infos_extra_1d =*(struct cpuid_0x80000001_edx*)&r.edx; // strict aliasing, undefined behavior, but easier like this
  printf("| Detected CPU features : \n");
#define X(f) print_state(#f, cpuid_infos_extra_1c.f);
  XCPUID_0x80000001C
#undef X

#define X(f) print_state(#f, cpuid_infos_extra_1d.f);
  XCPUID_0x80000001D
#undef X
  printf("\n");

}

INIT_ORDERED
{
INIT_ENTRY(cpuid_0x80000000)
INIT_ENTRY(cpuid_0x80000001)
INIT_ENTRY(cpuid_1)
};

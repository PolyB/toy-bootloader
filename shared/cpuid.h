#ifndef SHARED_CPUID_H
#define SHARED_CPUID_H

#include <stdint.h>


#define XPFEATURES1 \
  X(rdrand) X(f16c) X(avx) X(osxsave) X(xsave) X(aes) X(tsc_deadline)     \
  X(popcnt) X(movbe) X(x2apic) X(sse42) X(sse41) X(dca) X(pcid) X(pdcm)   \
  X(xtpr) X(cmpxchg16b) X(fma) X(sdbg) X(cnxt_id) X(ssse3) X(tm2) X(eist) \
  X(smx) X(vmx) X(ds_cpl) X(monitor) X(dtes64) X(pclmulqdq) X(sse3)       \
  X(pbe) X(tm) X(htt) X(ss) X(sse2) X(sse) X(fxsr) X(mmx) X(apci) X(ds)   \
  X(clfsh) X(psn) X(pse36) X(pat) X(cmov) X(mca) X(pge) X(mtrr) X(sep)    \
  X(apic) X(cx8) X(mce) X(pae) X(msr) X(tsc) X(pse) X(de) X(vme) X(fpu)

struct pfeatures // intel manual 2A-3-308
{
  uint8_t rdrand : 1;
  uint8_t f16c : 1;
  uint8_t avx : 1;
  uint8_t osxsave : 1;
  uint8_t xsave : 1;
  uint8_t aes : 1;
  uint8_t tsc_deadline : 1;
  uint8_t popcnt : 1;
  uint8_t movbe : 1;
  uint8_t x2apic : 1;
  uint8_t sse42 : 1;
  uint8_t sse41 : 1;
  uint8_t dca : 1;
  uint8_t pcid : 1;
  uint8_t _unused : 1;
  uint8_t pdcm : 1;
  uint8_t xtpr : 1;
  uint8_t cmpxchg16b : 1;
  uint8_t fma : 1;
  uint8_t sdbg : 1;
  uint8_t cnxt_id : 1;
  uint8_t ssse3 : 1;
  uint8_t tm2 : 1;
  uint8_t eist : 1;
  uint8_t smx : 1;
  uint8_t vmx : 1;
  uint8_t ds_cpl : 1;
  uint8_t monitor : 1;
  uint8_t dtes64 : 1;
  uint8_t pclmulqdq : 1;
  uint8_t sse3 : 1;
  uint8_t _unused2 : 1;
  // part 2
  uint8_t pbe : 1;
  uint8_t _unused3 : 1;
  uint8_t tm : 1;
  uint8_t htt : 1;
  uint8_t ss : 1;
  uint8_t sse2 : 1;
  uint8_t sse : 1;
  uint8_t fxsr : 1;
  uint8_t mmx : 1;
  uint8_t apci : 1;
  uint8_t ds : 1;
  uint8_t _unused4 : 1;
  uint8_t clfsh : 1;
  uint8_t psn : 1;
  uint8_t pse36 : 1;
  uint8_t pat : 1;
  uint8_t cmov : 1;
  uint8_t mca : 1;
  uint8_t pge : 1;
  uint8_t mtrr : 1;
  uint8_t sep : 1;
  uint8_t _unused5 : 1;
  uint8_t apic : 1;
  uint8_t cx8 : 1;
  uint8_t mce : 1;
  uint8_t pae : 1;
  uint8_t msr : 1;
  uint8_t tsc : 1;
  uint8_t pse : 1;
  uint8_t de : 1;
  uint8_t vme : 1;
  uint8_t fpu : 1;
} __attribute__((packed));

#define XCPUID_0x80000001C \
  X(lahf_sahf_available) X(lzcnt) X(prefetchw)

struct cpuid_0x80000001_ecx
{
  uint8_t lahf_sahf_available : 1;
  uint8_t _unused1 : 4;
  uint8_t lzcnt : 1;
  uint8_t _unused2 : 2;
  uint8_t prefetchw : 1;
  uint32_t _unused3 : 23;

} __attribute__((packed));

#define XCPUID_0x80000001D \
  X(syscall_sysret_64) X(exec_disable_bit) X(has_1Gbit_pages) X(rtdscp_ia32_tsc_aux) X(arch64_available)

struct cpuid_0x80000001_edx
{
  uint16_t _unused1 : 11;
  uint8_t syscall_sysret_64 : 1;
  uint8_t _unused2 : 8;
  uint8_t exec_disable_bit : 1;
  uint8_t _unused3 : 5;
  uint8_t has_1Gbit_pages : 1;
  uint8_t rtdscp_ia32_tsc_aux : 1;
  uint8_t _unused4 : 1;
  uint8_t arch64_available : 1;
  uint8_t _unused5: 2;
} __attribute__((packed));

extern uint16_t memory_size;
extern uint8_t clflush_cl;
extern uint8_t loc_apic_id;
extern struct pfeatures proc_features;
extern struct cpuid_0x80000001_ecx cpuid_infos_extra_1c;
extern struct cpuid_0x80000001_edx cpuid_infos_extra_1d;

extern uint32_t max_cpuid;

#endif

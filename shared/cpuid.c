#include "cpuid.h"

uint16_t memory_size;
uint8_t clflush_cl;
uint8_t loc_apic_id;
struct pfeatures proc_features;
uint32_t max_cpuid;
struct cpuid_0x80000001_ecx cpuid_infos_extra_1c;
struct cpuid_0x80000001_edx cpuid_infos_extra_1d;

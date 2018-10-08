#ifndef SHARED_MEM_H
#define SHARED_MEM_H

#include <stdint.h>

struct mem_sec
{
  uint64_t base;
  uint64_t length;
  uint32_t type;
  uint32_t acpi3;
} __attribute__((packed));

extern struct mem_sec memory_layout[50];

#endif

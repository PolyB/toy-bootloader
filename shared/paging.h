#ifndef SHARED_PAGING
#define SHARED_PAGING

#include <stdint.h>

struct pg_base
{
  uint8_t present : 1;
  uint8_t writable : 1;
  uint8_t user_accessible : 1;
  uint8_t write_through : 1;
  uint8_t disable_cache : 1;
  uint8_t accessed : 1;
  uint8_t dirty : 1;
  uint8_t huge_null : 1;
  uint8_t global : 1;
  uint8_t data : 1;
  uint64_t ptr : 42;
  uint32_t available : 11;
  uint8_t no_exec : 1;
}__attribute__((packed));

typedef struct pg_base pg_pml4; // page-mapo level 4 table
typedef struct pg_base pg_pdp; // page directory pointer table
typedef struct pg_base pg_pd; // page directory table
typedef struct pg_base pg_pt; // page table

#endif // SHARED_PAGING

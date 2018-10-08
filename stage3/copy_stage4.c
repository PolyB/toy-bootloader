#include "idt.h"
#include "printf.h"
#include "shared/floppy_read.h"
#include "init.h"

extern char _loader_protected_start;
extern char _loader_protected_first_section_index;
extern char _loader_protected_size;

void copy_stage4(void)
{
  size_t sector = (size_t)&_loader_protected_first_section_index;
  uint8_t *dest = (uint8_t*)&_loader_protected_start;
  long count = (size_t)&_loader_protected_size;
  printf("copying %d bytes from floppy disk sector %d to 0x%p\n", count, sector, dest);

  const char * err = fpy_copy_bytes(sector, dest, count);
  if (err)
    printf("error : %s\n", err);
  printf("success\n");
}

INIT(copy_stage4);

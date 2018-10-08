#include <stdint.h>
#include <stddef.h>
#include "shared/disk_props.h"
// 16bit real mode

extern uint8_t drive_number; // defined in boot_asm.s


// this function is used only to load stage2
void copy_kernel(uint8_t index, uint8_t count, void* dest)
{
  uint16_t tries = 3;
  uint16_t ax;

  uint8_t c = 0;
  uint8_t h = 0;
  uint8_t s = index + 1;
  if (max_head_number)
  {
    c = index / (max_head_number * max_sector_number);
    h = (index /  max_sector_number) % max_head_number;
    s = (index % max_sector_number) + 1;
  }

  do
  {
  __asm__ volatile("mov %%ax, %%es; int $0x13" : "=a"(ax) :"a"(0), "d"(drive_number):"memory");
  __asm__ volatile ("int $0x13" : "=a"(ax) :
                                  "a"(0x0200 | count),  // fun & count
                                  "d"((uint16_t)h << 8 | drive_number),  // head & drive number
                                  "c"(((c & 0xFF) << 8) | (c & 0x300)>>2 | (s & 0x3f)), // cylinder & starting sector number
                                  "b"(dest)
                                  : "cc", "memory");
  } while (--tries && (ax & 0xFF00));
  if (!tries)
  {
  // TODO error
  }

}

extern char _content_start; // linker variable
extern char _stage_2_first_section_index; // linker variable
extern char _stage_2_load_count; // linker variable
extern char _stage_3_first_section_index; // linker variable
extern char _stage_3_load_count; // linker variable
extern char _shared_start; // linker variable
extern char _shared_first_section_index; // linker variable
extern char _shared_load_count; // linker variable

void stage2_load(void); // stage2/load.c
void __attribute__((noreturn)) stage3_start(void); // stage3/start.s

void __attribute__((noreturn)) init(void)
{
  copy_kernel((size_t)&_shared_first_section_index, (size_t)&_shared_load_count, &_shared_start);
  copy_kernel((size_t)&_stage_2_first_section_index, (size_t)&_stage_2_load_count, &_content_start);
  stage2_load();
  copy_kernel((size_t)&_stage_3_first_section_index, (size_t)&_stage_3_load_count, &_content_start);
  stage3_start();
}

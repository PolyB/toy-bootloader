#include "printf.h"
#include "shared/idt_32.h"
#include "shared/floppy_read.h"
#include "write.h"


#define X_STAGES \
  X(stage_4)   \
  X(stage_5)

#define X(S)                              \
  extern char _##S##_first_section_index; \
  extern char _##S##_size;                \
  extern char _##S##_start;
X_STAGES
#undef X
extern char _content_start;
extern char _stage_5_start;

extern char _stage_4_bss_start;
extern char _stage_4_bss_end;

static void reset_bss(char *start, char *end)
{
  while (start < end) *(start++) = '\0';
}

void __attribute__((noreturn)) stage4_start(void);

void __attribute__((noreturn)) load_protected(void)
{
  fpy_setup_idt();
#define X(S) \
  fpy_copy_bytes((size_t)&_##S##_first_section_index, (void*)&_##S##_start, (size_t)&_##S##_size);
  X_STAGES
#undef X
  reset_bss(&_stage_4_bss_start, &_stage_4_bss_end);
  stage4_start();
}

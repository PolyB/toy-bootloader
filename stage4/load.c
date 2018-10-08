#include "printf.h"

void setup_idt(void);
void setup_paging(void);

void __attribute__((noreturn)) stage5_start();

int __attribute__((noreturn)) stage4_load()
{
  printf("stage4 loaded\n");
  // setup_idt();
  printf("idt loaded\n");
  setup_paging();
  printf("paging enabled\n");
  stage5_start();
}

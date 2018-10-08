#include "idt.h"
.code32


#define XNOERRORCODE pushl $0
#define XERRORCODE

#define X(N, XERREXPENSION, TEXT) \
.global idt_entry_32_##N       ;\
idt_entry_32_##N:              ;\
  cli                       ;\
  XERREXPENSION             ;\
  pushl $##N                ;\
  pushl %eax                ;\
  pushl %ebx                ;\
  pushl %ecx                ;\
  pushl %edx                ;\
  pushl %esi                ;\
  pushl %edi                ;\
  pushl %esp                ;\
  mov (idt_handlers_32 + (N * 4)), %eax   ;\
  call *%eax                ;\
  add $4, %esp              ;\
  popl %edi                 ;\
  popl %esi                 ;\
  popl %edx                 ;\
  popl %ecx                 ;\
  popl %ebx                 ;\
  popl %eax                 ;\
  add $8, %esp              ;\
  iret                      ;

X_IDT_ENTRIES

#undef X

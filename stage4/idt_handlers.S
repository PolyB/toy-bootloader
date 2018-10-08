#include "shared/idt.h"
.code64

#define XNOERRORCODE pushq $0
#define XERRORCODE

#define X(N, XERRORCODE, TEXT) \
.global idt_entry_64_##X             ;\
idt_entry_64_##X:                    ;\
cli                                  ;\
XERREXPENSION                        ;\
pushl $##N                           ;\
pushq %rax                           ;\
pushq %rcx                           ;\
pushq %rdx                           ;\
pushq %rbx                           ;\
pushq %rbp                           ;\
pushq %rsi                           ;\
pushq %rdi                           ;\
mov %esp, %eax                       ;\
mov (idt_handlers_64 + (N * 8), %rax ;\
callq *%rax                          ;\
iretq
// TODO 

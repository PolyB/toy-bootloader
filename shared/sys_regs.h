#ifndef SHARED_SYS_REGS
#define SHARED_SYS_REGS

#define _STR(A) #A

#define SET_BIT(REG, BIT)                           \
  __asm__ volatile ("mov %%" #REG ", %%eax;"        \
                    "or $"_STR(1 << (BIT))", %%eax;" \
                    "mov %%eax, %%" #REG            \
                    :::"eax");

#define SET_REG(REG, VAL)                           \
  __asm__ volatile ("mov %%eax, %%" #REG            \
                    ::"a"((VAL)):);

#endif

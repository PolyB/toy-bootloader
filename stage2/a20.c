#include <stdbool.h>

#include "init.h"
#include "int.h"
#include "print.h"
#include "printf.h"

static int a20_int15(void)
{
  printf("|| activate a20 with int15 ax=2401\n");
  __asm__ volatile ("int $0x15" ::"a"(0x2401):);
  uint16_t return_code;
  __asm__ volatile ("int $0x15" :"=a"(return_code):"a"(0x2402):);

  return return_code;
}

static void a20_keyboard_wait(bool n)
{
  uint8_t a;
  do
  {
    __asm__ volatile ("inb $0x64, %%al":"=a"(a)::);
  } while (n == ( a == 2));
}

static int a20_keyboard(void)
{
  printf("|| activate a20 with keyboard\n");
  a20_keyboard_wait(false);
  __asm__ volatile ("outb %%al, $0x64"::"a"(0xAD):);

  a20_keyboard_wait(false);
  __asm__ volatile ("outb %%al, $0x64"::"a"(0xD0):);

  uint8_t a;
  a20_keyboard_wait(true);
  __asm__ volatile ("inb $0x60, %%al":"=a"(a)::);

  a20_keyboard_wait(false);
  __asm__ volatile ("outb %%al, $0x64"::"a"(0xD1):);

  a20_keyboard_wait(false);
  __asm__ volatile ("outb %%al, $0x60"::"a"(a | 2):);

  a20_keyboard_wait(false);
  __asm__ volatile ("outb %%al, $0x64"::"a"(0xAE):);

  a20_keyboard_wait(false);
  return 1;
}

void a20(void)
{
  /* WITH int15 */
  if (a20_int15())
  {
    printf("|\\ success \n");
    return;
  }
  printf("|\\ failed\n");

  if (a20_keyboard())
  {
    printf("|\\ success \n");
    return;
  }
  printf("|\\ failed \n");

}


INIT(a20);

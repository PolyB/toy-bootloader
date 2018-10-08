#include "print.h"


struct cur_pos cursor_pos = { 0, 0};
static char cur_color = 0;

 static struct cur_pos __attribute__((unused)) getpos(void) 
{
  uint16_t d;
  __asm__ volatile("int $0x10" : "=d"(d) 
                               : "a"(0x0300),
                                 "b"(0x0000)
                               : "cx");
  struct cur_pos res = { d >> 8 , d & 0xFF};
  return res;
}

static void setpos(struct cur_pos p)
{
  __asm__ volatile("int $0x10" :: "a"(0x0200),
                                  "b"(0x0000),
                                  "d"((p.row << 8) | p.column));
}

static void writechar(char c)
{
  __asm__ volatile("int $0x10" :: "a"(0x0900 | c),
                                  "b"(0x0000 | cur_color),
                                  "c"(1));
}

void set_color(enum color c)
{
  cur_color = c;
}

enum color get_color(void)
{
  return cur_color;
}

void clear_screen(void)
{
  __asm__ volatile("int $0x10" :: "a"(0));
}

void putc(char c)
{
  if ((c & 0xF0) == 0xF0)
  {
    set_color(c & 0xF);
    return;
  }
  switch (c)
  {
  case '\n':
    cursor_pos.column = 0;
    cursor_pos.row++;
    break;
  default:
    setpos(cursor_pos);
    writechar(c);
    if (cursor_pos.column >= 80)
    {
      cursor_pos.column = 0;
      cursor_pos.row++;
    }
    else
    {
      cursor_pos.column++;
    }
  }
}

void puts(const char *s)
{
  while (*s)
  {
    putc(*s++);
  }
}

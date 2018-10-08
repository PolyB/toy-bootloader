#ifndef PRINT_H
#define PRINT_H

#include <stdint.h>
#include <stddef.h>

struct cur_pos
{
  uint8_t row;
  uint8_t column;
};

enum color
{
  black = 0,
  blue,
  green,
  cyan,
  red,
  magenta,
  brown,
  light_gray,
  dark_gray,
  light_blue,
  light_green,
  light_red,
  light_magenta,
  yellow,
  white
};

#define COLOR(X) (0xF0 | (X))

void clear_screen(void);
void putc(char c);
void puts(const char *c);
void set_color(enum color c);
enum color get_color(void);
extern struct cur_pos cursor_pos;


#endif

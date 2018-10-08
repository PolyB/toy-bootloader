#include <stdint.h>

#include "printf.h"
#include "print.h"
#include "init.h"

extern struct init _stage2_init_start;
extern struct init _stage2_init_end;

static struct cur_pos progress_cursor;

static void update_progress_bar(uint16_t curr, uint16_t count)
{
  struct cur_pos base_cur = cursor_pos;
  enum color base_color = get_color();
  set_color(yellow);
  cursor_pos = progress_cursor;
  putc('[');
  for (uint16_t i = 1; i <= count; i++)
  {
    if (i < curr)
      putc('=');
    else if (i == curr)
      putc('>');
    else
      putc(' ');
  }
  putc(']');
  cursor_pos = base_cur;
  set_color(base_color);
}

void stage2_load(void)
{
  // clear_screen();
  puts("stage 2 loaded\n");
  set_color(light_gray);

  puts("executing stage 2 : ");
  progress_cursor = cursor_pos;
  puts("\n");
  uint16_t task_count = (&_stage2_init_end - &_stage2_init_start);
  for (struct init *f = &_stage2_init_start; f < &_stage2_init_end; f++)
  {
    uint16_t current_task = 1 + (f - &_stage2_init_start);
    update_progress_bar(current_task, task_count);
    printf("executing `%c%s%c` in `%c%s%c` (%c%hu%c of %c%hu%c)\n",
          COLOR(blue), f->name, COLOR(light_gray),
          COLOR(blue), f->file,COLOR(light_gray),
          COLOR(blue), current_task, COLOR(light_gray),
          COLOR(blue), task_count, COLOR(light_gray)
          );
    f->fun();
    printf("done\n\n");
  }
  update_progress_bar(task_count+1, task_count);

  printf("stage 2 finished\n\n ");

}

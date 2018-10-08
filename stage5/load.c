#include "init.h"
#include "printf.h"
#include "write.h"

extern struct init _stage5_init_start;
extern struct init _stage5_init_end;


void stage5_load()
{
  printf("\nnow in stage 5 (64 bit)\n");
  printf("loading modules\n");
  for(struct init *i = &_stage5_init_start; i < &_stage5_init_end;i++)
  {
    printf("executing `%s` in `%s` (%u of %u)\n",
        i->name, i->file, &_stage5_init_end - i, &_stage5_init_end - &_stage5_init_start);
    i->fun();
  }
  
  printf("finished\n");
  for(;;);
} 

#ifndef INIT_H
#define INIT_H


struct init {
  void (*fun)(void);
  const char *name;
  const char *file;
};

#define INIT(F) \
  struct init init_##F __attribute__((section(".stage2_init"))) = { F, #F, __FILE__ };

#define _INIT_CONCAT2(N) init_ord_##N
#define _INIT_CONCAT(N) _INIT_CONCAT2(N)

#define INIT_ORDERED \
struct init _INIT_CONCAT(__LINE__)[] __attribute__((section(".stage2_init"))) =

#define INIT_ENTRY(F) \
  { F, #F, __FILE__ },


#endif

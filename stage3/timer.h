#ifndef TIMER_H
#define TIMER_H

void sleep(uint32_t); // this sleep is precice at ~1s
void sleep_exit(void); // exit current sleep() (usually called from and int handler)

#endif

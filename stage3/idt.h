#ifndef IDT_H
#define IDT_H

#include <stdint.h>

#include "shared/idt_32.h"

void interrupt_panic(struct int_infos*);
void interrupt_none(struct int_infos*);

void irq_mask(uint8_t line);
void irq_unmask(uint8_t line);


#endif

#ifndef SHARED_IDT_32_H
#define SHARED_IDT_32_H

#include <stdint.h>

#include "gdt.h"
#include "idt.h"

struct int_infos {
	uint32_t edi;
	uint32_t esi;
	uint32_t edx;
	uint32_t ecx;
	uint32_t ebx;
	uint32_t eax;
	uint32_t int_nbr;
	uint32_t ret_code;
	uint32_t ip;
} __attribute__((packed));

struct idt_event {
		uint16_t offset0 : 16;
		struct seg_register seg_selector;
		uint8_t _r0: 5;
		uint8_t idt_flag : 8;
		uint8_t dpl : 2;
		uint8_t p : 1;
		uint16_t offset1 : 16;
} __attribute__((packed));

typedef void (*idt_handler_fptr)(struct int_infos*);

extern struct idt_event g_idt[256];
extern idt_handler_fptr idt_handlers_32[256];

static inline void set_interrupt(uint8_t n, idt_handler_fptr f) { idt_handlers_32 [n] = f; }

#define X(N, err, msg) \
  void idt_entry_32_##N();
X_IDT_ENTRIES
#undef X

#endif

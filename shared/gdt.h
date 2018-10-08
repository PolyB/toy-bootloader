#ifndef SHARED_GDT_H
#define SHARED_GDT_H

#include <stdint.h>

struct gdt_seg_descriptor
{
	uint16_t seg_limit_1 : 16;
	uint16_t base_address_1 : 16;
	uint8_t base_address_2 : 8;
  uint8_t accessed : 1;
  uint8_t readable_writable : 1;
  uint8_t direction_conforming : 1;
  uint8_t code_sel : 1;

	uint8_t _true : 1;
	uint8_t privilelge_level : 2;
	uint8_t segment_present : 1;
	uint8_t seg_limit_2 : 4;
	uint8_t available_system_software: 1;
	uint8_t mode_64_bit : 1;
	uint8_t op_size : 1;
	uint8_t granularity : 1;
	uint8_t base_address_3 : 8;
} __attribute__((packed));


struct seg_register
{
	uint8_t rpl : 2;
	uint8_t ti : 1;
	uint16_t index : 13;
} __attribute__((packed));

extern struct gdt_seg_descriptor gdt[4];

extern struct seg_register seg_ss;
extern struct seg_register seg_es;
extern struct seg_register seg_fs;
extern struct seg_register seg_gs;
extern struct seg_register seg_ds;
extern struct seg_register seg_cs;

#endif

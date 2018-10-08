#include "gdt.h"

struct gdt_seg_descriptor gdt[4];
struct seg_register seg_ss;
struct seg_register seg_es;
struct seg_register seg_fs;
struct seg_register seg_gs;
struct seg_register seg_ds;
struct seg_register seg_cs;

#include "idt_32.h"

struct idt_event g_idt[256];

idt_handler_fptr idt_handlers_32[256];

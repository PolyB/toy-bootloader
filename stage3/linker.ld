ENTRY(stage3_start)

SECTIONS
{
  .stage3 : {
      *(.text)
      _stage3_idt_begin = .;
      *(.text.idt)
      _stage3_idt_end = .;
      *(.data)
      *(.rodata*)
  }
  .bss.stage3 : {
    *(.bss)
  }
  .stage3.init : SUBALIGN(1) /* remove linker alignement between fields*/ {
    _stage3_init_start = .;
    idt.o(.stage3.init)
    breakpoint.o(.stage3.init)
    timer.o(.stage3.init)
    floppy.o(.stage3.init)
    *(.stage3.init)
    _stage3_init_end = .;

  }

}

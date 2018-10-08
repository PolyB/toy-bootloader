ENTRY(stage2_load)

SECTIONS
{
  .stage2 : {
      *(.text)
      *(.data)
      *(.bss)
      *(.rodata*)
  }
  .stage2.init : SUBALIGN(1) /* remove linker alignement between fields*/ {
    _stage2_init_start = .;
    *(.stage2_init)
    _stage2_init_end = .;

  }

}

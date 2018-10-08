ENTRY(stage5_start)

SECTIONS
{
  .stage5 : {
      *(.text)
      *(.data)
      *(.bss)
      *(.rodata*)
  }
  .stage5.init : SUBALIGN(1)/* remove linker alignement between fields*/{
    _stage5_init_start = .;
    *(.stage5.init)
    _stage5_init_end = .;

  }

}

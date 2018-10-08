ENTRY(stage4_start)

SECTIONS
{
  .stage4 : {
      *(.text)
      *(.data)
      *(.rodata*)
  }
  .bss.stage4 : {
    *(.bss)
  }

}

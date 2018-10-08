ENTRY(main_asm)

SECTIONS
{
  .stage1 : {
    *(.text_startup)
    *(.text)
    *(.data)
    *(.bss)
    *(.rodata*)
  }
}

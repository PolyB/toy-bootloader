SECTIONS
{
  .data_shared.load :
  {
    *(.text)
    *(.data)
    *(.rodata)
  }
  .data_shared.bss :
  {
    *(.bss)
  }
}

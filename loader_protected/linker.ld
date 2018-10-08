ENTRY(load_protected)

SECTIONS
{
  .loader_protected : {
      *(.text)
      *(.data)
      *(.bss)
      *(.rodata*)
  }

}

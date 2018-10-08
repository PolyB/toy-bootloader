.code16
.section .text_startup
.globl main_asm
main_asm: 
  cli

  xor     %ax, %ax
  mov     %ax, %ss
  mov     $0x7000,%sp
  mov     %ax, %es
  mov     %ax, %ds

  mov %dl, drive_number

  jmpl $0, $init

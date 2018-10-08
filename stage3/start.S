.code16
.globl stage3_start
stage3_start:

cli

movl %cr0, %eax
or $1, %al
movl %eax, %cr0


movw (seg_ds), %ds
movw (seg_ss), %ss
movw (seg_es), %es
movw (seg_fs), %fs
movw (seg_gs), %gs

pushw (seg_cs)
pushw $1f

lret
1:

.code32

call stage3_load


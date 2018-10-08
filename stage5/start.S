.code32
.globl stage5_start
stage5_start:

cli

movw (seg_ss), %ss
movw (seg_es), %es
movw (seg_fs), %fs
movw (seg_gs), %gs
movw (seg_ds), %ds
pushw (seg_cs)
push $1f
lret
1:

.code64
jmp stage5_load

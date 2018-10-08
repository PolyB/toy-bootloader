# build and run
```
make
qemu-system-x86_64 -enable-kvm -fda boot.bin -nographic -serial stdio -monitor none
```
# bootloader
## bootloader stages
### shared
contains the global value and headers shared between bootloader stages and the kernel

## stage1 (real mode - 16bit)
this is the first 512 octets of the bootloader, it's purpose is to load the next stages of the bootloader
* read the stage2 from the floppy disk
* execute stage2
* overwrite the stage2 with the stage3
* execute stage3

## stage2 (real mode - 16bit)
execute the 16bit tasks

## stage3 (protected-32)
enter 32bit mode (stage3/start.S)

execute the 32bit tasks

One of the task is read from the floppy disk the `loader_protected` in memory

the stage ends by jumping on `loader_protected`
## loader\_protected (protected-32)
this stage just reads the stage 4 and 5 from the floppy disk and execute them

## stage4 (long-32)
the stage4 loads the datas nessesary for 64bit long mode

## stage5 (long-64)
the stage5 is long-64 mode, the kernel is here.

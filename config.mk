LDFLAGS=-static -nostdlib --nmagic
QEMU=qemu-system-x86_64
QEMUFLAGS= -nographic -serial stdio -monitor none -enable-kvm # -cpu host
CC=gcc
CFLAGS=-march=x86-64 -ffreestanding -Wall -fno-pie -fno-stack-protector -fno-asynchronous-unwind-tables -Os -fno-common -Wno-packed-bitfield-compat -fno-builtin -mno-sse -std=c11 -nostdlib -std=c11
LDFLAGS=-static -nostdlib --nmagic -Tlinker.ld
OBJCOPYFLAGS = -O elf64-x86-64
ASFLAGS=-march=i386

#
# Stage-specific
#

ifdef STAGE

$(STAGE).o: stage_summary linker.ld $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)
	objcopy $(OBJCOPYFLAGS) $@

CFLAGS += -I..
LDFLAGS += -r -Map=../$(STAGE).map


STAGE_SUMMARY= "\n ==========[stage]===========\n"
STAGE_SUMMARY +="| name : $(STAGE)\n"

stage_summary:
	@if [ -t 1 ]; then echo -en "\e[1m"; fi
	@echo -e $(STAGE_SUMMARY)
	@if [ -t 1 ]; then echo -en "\e[21m"; fi


ifdef STAGE_ENTRY
OBJCOPYFLAGS += -G $(STAGE_ENTRY)
STAGE_SUMMARY +="| entry : $(STAGE_ENTRY)\n"
endif #STAGE_ENTRY



endif # STAGE

#
# CODE SIZE
#
ifdef CODE_SIZE
STAGE_SUMMARY +="| code_size : $(CODE_SIZE)\n"
endif

ifeq ($(CODE_SIZE), 16)
# CODE_SIZE=16
CFLAGS += -m16
ASFLAGS += --32
LDFLAGS += --oformat=elf32-i386 -m elf_i386
endif

ifeq ($(CODE_SIZE), 32)
# CODE_SIZE=32
CFLAGS += -m32
ASFLAGS += --32
LDFLAGS += --oformat=elf32-i386 -m elf_i386
endif

ifeq ($(CODE_SIZE), 64)
# CODE_SIZE=64
CFLAGS += -m64

ASFLAGS += --64
LDFLAGS += --oformat=elf64-x86-64
endif

%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@

#  CFLAGS += -DDEBUG

clean:
	$(RM) *.map
	$(RM) $(OBJS)
	$(CLEAN)

STAGE_SUMMARY +="| CFLAGS : $(CFLAGS)\n"
STAGE_SUMMARY +="| LDFLAGS : $(LDFLAGS)\n"
STAGE_SUMMARY +="| OBJCOPYFLAGS : $(OBJCOPYFLAGS)\n"
STAGE_SUMMARY +="| ASFLAGS : $(ASFLAGS)\n"
STAGE_SUMMARY +="============================\n"

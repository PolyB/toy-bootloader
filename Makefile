MAIN_TARGET=boot

include config.mk

all: $(MAIN_TARGET).bin

STAGES= shared stage1 stage2 stage3 loader_protected stage4 stage5

OBJS= $(foreach var,$(STAGES), $(var)/$(var).o)

.PHONY: $(STAGES) test debug test-monitor stage_summary clean $(OBJS)

ifdef EXTRA_OBJECT
EXTRA_OBJECTS_PATH=$(shell readlink -f $(EXTRA_OBJECT))
endif



$(MAIN_TARGET).bin: $(OBJS)
	$(LD) $(LDFLAGS) -Map=boot.map -o $@ -b elf64-x86-64 $^

test: $(MAIN_TARGET).bin
	$(QEMU) $(QEMUFLAGS) -fda $^  

$(MAIN_TARGET)l.bin: $(MAIN_TARGET).bin
	dd if=/dev/zero of=$@ bs=512 count=2880
	dd if=$^ of=$@ conv=notrunc

$(MAIN_TARGET).elf: $(OBJS)
	$(LD) $(LDFLAGS) --oformat elf64-x86-64 -Map=boot.map -o $@ -b elf64-x86-64 $^

define MK_STAGE
$1/$1.o:
	EXTRA_OBJECTS_PATH=$(EXTRA_OBJECTS_PATH) $(MAKE) $1.o -C $1
endef

$(foreach stage,$(STAGES), $(eval $(call MK_STAGE,$(stage))))

stage_summary:
	$(foreach stage, $(STAGES), $(MAKE) -C $(stage) stage_summary;)


define CLEAN
	$(foreach stage, $(STAGES), $(MAKE) -C $(stage) clean;)
	$(RM) *.map
	$(RM) $(MAIN_TARGET).bin
	$(RM) $(MAIN_TARGET)l.bin
	$(RM) $(MAIN_TARGET).elf
endef

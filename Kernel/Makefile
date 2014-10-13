# Makefile for SimpleOS Kernel

OUTPUT_FILE := ../bin/kernel_elf
SOURCE_DIRS := ../Shared/sources/ ./all/ ./arch/ia32/
CSOURCES := $(shell find $(SOURCE_DIRS) -name "*.c")
SSOURCES := $(shell find $(SOURCE_DIRS) -name "*.s")
ALLFILES := $(CSOURCES) $(SSOURCES)

OBJECTS := $(shell find $(SOURCE_DIRS) -name "*.o")
SOURCES := $(patsubst %.s,%.o,$(SSOURCES)) $(patsubst %.c,%.o,$(CSOURCES))

CC=gcc
CFLAGS=-nostdlib -nostdinc -fno-builtin -I ./all/ -I ../Shared/headers -I ./arch/ia32/ -fno-stack-protector -m32 -Wall -D _ARCH_IA32_=1 -D _ARCH_AMD64_=2 -D _ARCH_=1
LDFLAGS=-Tlink.ld -melf_i386
ASFLAGS=-felf32

all: $(SOURCES) link

clean:
	-@rm $(OBJECTS) $(OUTPUT_FILE)

sources:
	@echo $(SSOURCES)
	@echo $(CSOURCES)

link:
	@ld $(LDFLAGS) -o $(OUTPUT_FILE) $(SOURCES)

todo:
	-@for file in $(ALLFILES); do fgrep -H -e TODO -e FIXME $$file; done; true

maps:
	-@for file in $(ALLFILES); do fgrep -H -e map $$file; done; true	

.s.o:
	@nasm $(ASFLAGS) $<
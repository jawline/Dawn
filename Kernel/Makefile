# Makefile for SimpleOS Kernel

OUTPUT_FILE := ../bin/kernel_elf
PROJDIRS := ./sources ../Shared/sources/
CSOURCES := $(shell find $(PROJDIRS) -name "*.c")
SSOURCES := $(shell find $(PROJDIRS) -name "*.s")
ALLFILES := $(CSOURCES) $(SSOURCES)

OBJECTS := $(shell find $(PROJDIRS) -name "*.o")

SOURCES := $(patsubst %.s,%.o,$(SSOURCES)) $(patsubst %.c,%.o,$(CSOURCES))

CC=gcc
CFLAGS=-nostdlib -nostdinc -fno-builtin -I ./headers -I ../Shared/headers -fno-stack-protector -m32 -Wall
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

todso:
	-@for file in $(ALLFILES); do fgrep -H -e allocateFrame $$file; done; true

.s.o:
	@nasm $(ASFLAGS) $<

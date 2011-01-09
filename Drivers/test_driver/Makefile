# Makefile for SimpleOS Driver

OUTPUT_FILENAME := ./Build/test_driver

PROJDIRS := ./sources
CSOURCES := $(shell find $(PROJDIRS) -name "*.c")
SSOURCES := $(shell find $(PROJDIRS) -name "*.s")
ALLFILES := $(CSOURCES) $(SSOURCES)

OBJECTS := $(shell find $(PROJDIRS) -name "*.o")

SOURCES := $(patsubst %.s,%.o,$(SSOURCES)) $(patsubst %.c,%.o,$(CSOURCES))

CC=gcc
CFLAGS=-nostdlib -nostdinc -fno-builtin -I ./headers -I ../Shared/headers -fno-stack-protector -m32
LDFLAGS=-melf_i386 -r
ASFLAGS=-felf32

all: $(SOURCES) link

clean:
	-@rm $(OBJECTS) $(OUTPUT_FILENAME)

sources:
	@echo $(SSOURCES)
	@echo $(CSOURCES)

link:
	@ld $(LDFLAGS) -o $(OUTPUT_FILENAME) $(SOURCES)
.s.o:
	@nasm $(ASFLAGS) $<

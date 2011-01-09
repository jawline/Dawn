# Makefile for a SimpleOS program

API_DIR := ../../API/
SHARED_DIR := ../../Shared/

OUTPUT_FILE := ./Build/clear

PROJDIRS := ./sources $(API_DIR)/sources/ $(SHARED_DIR)/sources/
CSOURCES := $(shell find $(PROJDIRS) -name "*.c")
SSOURCES := $(shell find $(PROJDIRS) -name "*.s")
ALLFILES := $(CSOURCES) $(SSOURCES)

OBJECTS := $(shell find $(PROJDIRS) -name "*.o")

SOURCES := $(patsubst %.s,%.o,$(SSOURCES)) $(patsubst %.c,%.o,$(CSOURCES))

CC=g++
CFLAGS=-nostdlib -nostdinc -fno-builtin -I ./headers -I $(API_DIR)/headers -I $(SHARED_DIR)/headers/ -fno-stack-protector -m32 -fno-exceptions
LDFLAGS=-melf_i386
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

.s.o:
	@nasm $(ASFLAGS) $<

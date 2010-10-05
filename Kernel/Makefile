# Makefile for SimpleOS Kernel

OUTPUT_FILE := ./Build/Kernel
PROJDIRS := ./sources ../Shared/sources/
CSOURCES := $(shell find $(PROJDIRS) -name "*.c")
SSOURCES := $(shell find $(PROJDIRS) -name "*.s")
ALLFILES := $(CSOURCES) $(SSOURCES)

OBJECTS := $(shell find $(PROJDIRS) -name "*.o")

SOURCES := $(patsubst %.s,%.o,$(SSOURCES)) $(patsubst %.c,%.o,$(CSOURCES))

CC=gcc
CFLAGS=-nostdlib -nostdinc -fno-builtin -I ./headers -I ../Shared/headers -fno-stack-protector -m32
LDFLAGS=-Tlink.ld -melf_i386
ASFLAGS=-felf32

all: $(SOURCES) link

clean:
	-@rm $(OBJECTS) ./Build/Kernel

sources:
	@echo $(SSOURCES)
	@echo $(CSOURCES)

link:
	@ld $(LDFLAGS) -o $(OUTPUT_FILE) $(SOURCES)

todo:
	-@for file in $(ALLFILES); do fgrep -H -e TODO -e FIXME $$file; done; true

userpages:
	-@for file in $(ALLFILES); do fgrep -H -e PAGE_USER -e FIXME $$file; done; true

present_maps:
	-@for file in $(ALLFILES); do fgrep -H -e PAGE_PRESENT -e FIXME $$file; done; true

rfs_dirs_created:
	-@for file in $(ALLFILES); do fgrep -H -e create_rfs_directory $$file; done; true

list_dirs:
	-@for file in $(ALLFILES); do fgrep -H -e print_directory $$file; done; true

maps:
	-@for file in $(ALLFILES); do fgrep -H -e map $$file; done; true
irets:
	-@for file in $(ALLFILES); do fgrep -H -e iret $$file; done; true
usermode:
	-@for file in $(ALLFILES); do fgrep -H -e switchToUserMode $$file; done; true
simpleosfiles:
	-@for file in $(ALLFILES); do fgrep -H -e SimpleOS $$file; done; true


.s.o:
	@nasm $(ASFLAGS) $<

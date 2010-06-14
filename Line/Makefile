# Makefile for SimpleOS Line

PROJDIRS := ./ ../Shared/sources/ ../API/sources/
CSOURCES := $(shell find $(PROJDIRS) -name "*.c")
SSOURCES := $(shell find $(PROJDIRS) -name "*.s")
ALLFILES := $(CSOURCES) $(SSOURCES)

OBJECTS := $(shell find $(PROJDIRS) -name "*.o")

SOURCES := $(patsubst %.s,%.o,$(SSOURCES)) $(patsubst %.c,%.o,$(CSOURCES))

CC=gcc
CFLAGS=-nostdlib -nostdinc -fno-builtin -I ../Shared/headers -I ../API/headers/ -fno-stack-protector -m32
LDFLAGS=-Tlink.ld -melf_i386
ASFLAGS=-felf32

all: $(SOURCES) link

clean:
	-@rm $(OBJECTS) ./Build/Line

sources:
	@echo $(SSOURCES)
	@echo $(CSOURCES)

link:
	@ld $(LDFLAGS) -o ./Build/Line $(SOURCES)

todo:
	-@for file in $(ALLFILES); do fgrep -H -e TODO -e FIXME $$file; done; true

.s.o:
	@nasm $(ASFLAGS) $<

#
# Makefile
#
# Copyright (C) 2026 MikeM64
# 
# Licensed under the terms of GNU GPL, version 2
# http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
#

PREFIX ?= powerpc64-ps3-elf-
CFLAGS ?= -Wall -Werror -Wpedantic -ffreestanding -std=gnu99 -mbig-endian -O1 -mcpu=cell -m64\
	  -Iinclude/

LDFLAGS ?= -melf64ppc -T src/main.ld --print-map
ASFLAGS ?= -D__ASSEMBLY__ -Iinclude/

CC = $(PREFIX)gcc
CXX = $(PREFIX)gcc
AS = $(PREFIX)as
LD = $(PREFIX)ld
AR = $(PREFIX)ar
OBJCOPY = $(PREFIX)objcopy

.PHONY: clean
.DEFAULT_GOAL := all

SRC := $(shell find . -name "*.c" -or -name "*.S")

# Two Pass substitution for object names
OBJ_C := $(SRC:.c=.o)
OBJ = $(OBJ_C:.S=.o)

clean:
	rm -rf src/*.o
	rm -rf *.elf

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.s
	$(AS) $(ASFLAGS) -c -o $@ $<

ps3-sb-uart-loader.elf: $(OBJ)
	$(LD) -o ps3-sb-uart-loader.elf $(LDFLAGS) $(OBJ)

all: ps3-sb-uart-loader.elf


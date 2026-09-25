#
# Makefile
#
# Copyright (C) 2026 MikeM64
# 
# Licensed under the terms of GNU GPL, version 2
# http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
#

PREFIX ?= powerpc64-ps3-elf-
CFLAGS ?= -Wall -Werror -Wpedantic -ffreestanding -fpie -std=gnu99 -mbig-endian -O1 -mcpu=cell -m64\
	  -Iinclude/

LDFLAGS ?= -melf64ppc -pie --print-map
ASFLAGS ?= -mregnames -mbig-endian -mcell -Iinclude/

CC = $(PREFIX)gcc
CXX = $(PREFIX)gcc
AS = $(PREFIX)as
LD = $(PREFIX)ld
AR = $(PREFIX)ar
OBJCOPY = $(PREFIX)objcopy

.PHONY: clean
.DEFAULT_GOAL := all

SRC_C := $(shell find . -name "*.c")
SRC_S := $(shell find . -name "*.S")
SRC := $(SRC_C) $(SRC_S)

OBJ_C := $(SRC_C:.c=.o)
OBJ_S := $(SRC_S:.S=.o)
OBJ := $(OBJ_S) $(OBJ_C)

OBJ_BADWDSD_S := src/start_badwdsd.o
OBJ_ELF_S := src/start.o

clean:
	rm -rf src/*.o
	rm -rf *.elf
	rm -rf ps3-sb-uart-loader.badwdsd.bin

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.S
	$(AS) $(ASFLAGS) -o $@ $<

ps3-sb-uart-loader.elf: $(OBJ_C) $(OBJ_ELF_S)
	$(LD) -o ps3-sb-uart-loader.elf $(LDFLAGS) -T src/main.ld $(OBJ_ELF_S) $(OBJ_C)

ps3-sb-uart-loader.badwdsd.bin: $(OBJ_C) $(OBJ_BADWDSD_S)
	$(LD) -o ps3-sb-uart-loader.badwdsd.bin $(LDFLAGS) -T src/badwdsd.ld $(OBJ_BADWDSD_S) $(OBJ_C)


all: ps3-sb-uart-loader.elf ps3-sb-uart-loader.badwdsd.bin


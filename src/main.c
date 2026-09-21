/*
 * main.c - PS3 SB UART Loader
 * 
 * Copyright (C) 2026 MikeM64
 *
 * Licensed under the terms of the GNU GPL, version 2
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 */

#include <stdbool.h>

#include "types.h"
#include "elfloader.h"
#include "sb_printf.h"
#include "sb_uart.h"

int main (int argc, char *argv[])
{
    int  rc;
    u8  *elf_buffer = NULL;

    sb_init();

    INFO_LOG("PS3 SB UART Loader Started!");

    while (true) {
        rc = elfloader_sb_load(elf_buffer);
        if (rc == 0) {
            elfloader_file_launch(elf_buffer);
        }
    }

    return 0;
}


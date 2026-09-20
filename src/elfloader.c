/*
 * elfloader.c - lvx elf loader.
 * 
 * Copyright (C) 2012 jestero
 * 
 * Licensed under the terms of the GNU GPL, version 2
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 */

#include <errno.h>
#include <stdbool.h>

#include "compiler.h"
#include "elf.h"
#include "minilzo.h"
#include "sb_printf.h"
#include "sb_recvfile.h"
#include "sb_uart.h"
#include "sleep.h"
#include "string.h"

/* minilzo.h *************************************************/

unsigned char __LZO_MMODEL *in;
unsigned char __LZO_MMODEL *out;

/*************************************************************/

static bool _elfloader_check_elf(Elf64_Ehdr *ehdr)
{
    //Check ELF magic.
    if(ehdr->e_ident[EI_MAG0] != ELFMAG0 ||
       ehdr->e_ident[EI_MAG1] != ELFMAG1 ||
       ehdr->e_ident[EI_MAG2] != ELFMAG2 ||
       ehdr->e_ident[EI_MAG3] != ELFMAG3) {
        ERROR_LOG("ELF Magic incorrect!");
        return false;
    }

    //Check ELF class.
    if(ehdr->e_ident[EI_CLASS] != ELFCLASS64) {
        ERROR_LOG("ELF Class incorrect!");
        return false;
    }

    return true;
}

static bool elfloader_load(const u8 *buffer, u64 *elf)
{
    u32 i;
    Elf64_Ehdr *ehdr = (Elf64_Ehdr *)buffer;
    Elf64_Phdr *phdr = (Elf64_Phdr *)(buffer + ehdr->e_phoff);

    //Check ELF.
    if(_elfloader_check_elf(ehdr) == false)
    {
        ERROR_LOG("check_elf failed\n");
        return false;
    }

    INFO_LOG("Num program headers: %#x\n", ehdr->e_phnum);
    //Walk program headers.
    for(i = 0; i < ehdr->e_phnum; i++)
    {
        INFO_LOG("Program header %02d (type %d) \n", i,
                 phdr[i].p_type);

        //Only load PT_LOAD headers.
        if(phdr[i].p_type == PT_LOAD)
        {
            //Probably a good idea to check that (we only want real mode ELFs).
            if(phdr[i].p_vaddr != phdr[i].p_paddr)
            {
                ERROR_LOG("Error: vaddr (=0x%08x) != paddr (=0x%08x)\n",
                          phdr[i].p_vaddr, phdr[i].p_paddr);
                return false;
            }

            //Copy data.
            memcpy((void*) phdr[i].p_vaddr, buffer + phdr[i].p_offset, phdr[i].p_memsz);

            INFO_LOG("Loaded (@ 0x%08x, 0x%08x bytes)",
                     (u32)phdr[i].p_vaddr, phdr[i].p_memsz);
        }
        else
            INFO_LOG("Discarded.");
    }

    //Return ELF entry point.
    if (elf) {
        *elf = ehdr->e_entry;
    }
    return true;
}

static int elfloader_decompress(u8 *input_buffer, lzo_uint comp_length, lzo_uint orig_length, u8 *output_buffer)
{
    int r;
    lzo_uint new_len;

    // setting up input & output buffers
    in = input_buffer;
    out = output_buffer;

    if (out == NULL) {
        return ENOMEM;
    }

    INFO_LOG("LZO buffers initialized");

    if (lzo_init() != LZO_E_OK)
    {
        INFO_LOG("internal error - lzo_init() failed !!!");
        sb_printf("(this usually indicates a compiler bug - try recompiling\n"
                  "without optimizations, and enable '-DLZO_DEBUG' for diagnostics)\n");
        return ENOTSUP;
    }


    new_len = orig_length;
    INFO_LOG("LZO Initialized");
    r = lzo1x_decompress(in,comp_length,out,&new_len,NULL);
    if (r == LZO_E_OK && new_len == orig_length)
    {
        INFO_LOG("decompressed %i bytes back into %i bytes",
                 (unsigned long) comp_length, (unsigned long) orig_length);
    }
    else
    {
        /* this should NEVER happen */
        ERROR_LOG("internal error - decompression failed: %x", r);
        ERROR_LOG("output length: 0x%x", new_len);
        return EINVAL;
    }

    return EOK;
}

int elfloader_sb_load(u8 *buffer_location)
{
    // receive length of the original file
    u32 orig_length;
    sb_getN((u8*) &orig_length, 0x4);

    // receive length of the compressed file
    u32 comp_length;
    sb_getN((u8*) &comp_length, 0x4);

    // prepare the buffer
    u8 *buffolo = (u8*) 0xa000000ull;
    memset(buffolo, 0x00, comp_length);

    u8 *decomp_buffer = (u8 *) 0xb000000ull;

    if (orig_length > (uintptr_t)(decomp_buffer - buffolo)) {
        ERROR_LOG("File is too large (%u) - Max supported file length is %u",
                  orig_length, (uintptr_t)(decomp_buffer - buffolo));
        return ENOMEM;
    }

    // receive the file
    int st = fileReceive(buffolo, comp_length);

    // wait a bit to let me open minicom again...
    sleep(15);

    if (st != EOK) {
        ERROR_LOG("Fail receiving file, returned: %d", st);
        buffer_location = NULL;
        return EAGAIN;
    }
    else  {
        INFO_LOG("Success receiving file, returned: %d", st);
    }

    INFO_LOG("Compressed file length is: %i bytes", comp_length);

    // decompress the file
    st = elfloader_decompress(buffolo, comp_length, orig_length, decomp_buffer);

    if (st){
        buffer_location = NULL;
        ERROR_LOG("Failed decompressing file %d", st);
        return ENOMEM;
    }

    // setting buffolo with our decompressed elf addr
    buffolo = out;
    buffer_location = buffolo;

    return EOK;
}

static void elfloader_run_elf(u64 entry_point)
{
    INFO_LOG("Jumping to entry point: 0x%x", entry_point);

    // jump to entry point
    _ASMV("sync\n\t"
            "mtctr %0\n\t" /* move to counter */
            "bctr\n\t"     /* Branch to counter */
            :
            :"r"(entry_point)
            :"ctr"
            );
}

void elfloader_file_launch(const u8 *buffolo)
{
    u64 elf_entry_point;
    INFO_LOG("Launching ELF File...");
    if(elfloader_load(buffolo, &elf_entry_point)) {
        elfloader_run_elf(elf_entry_point);
    } else {
        ERROR_LOG("elfloader_load failed!");
    }
}



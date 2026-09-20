/*
 * sb_recvfile.c
 *
 *  Created on: 04/07/2012
 *      Author: jester
 *
 * Licensed under the terms of the GNU GPL, version 2
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 */

#include "types.h"
#include "sb_printf.h"
#include "sb_recvfile.h"
#include "sb_uart.h"

int handshake(void)
{
    sb_putc(SYN);
    u8 reply = sb_getc();
    if (reply == ACK)
        return EOK;
    return EBUSY;    
}

static u64 gen_chksum(u8 *addr, int size)
{
    int i;
    u32 chksum = 0;
    
    for (i = 0; i < size; i++)
        chksum += *addr++;
    
    return chksum;
}

int fileReceive(u8 *dest, int destsize)
{
    u8 stuff;
    u32 rounds;
    u64 chksum = 0;
    u8 *dest2 = dest;
    
    rounds = destsize / 0x10;
    
    while (rounds--)
    {
        if (handshake() != EOK)
            return EBUSY;
        sb_getN(dest, 0x10);
        dest += 0x10;
    }
    
    rounds = destsize % 0x10;
    
    if (rounds)
    {
        if (handshake() != 0)
            return EBUSY;
    
        sb_getN(dest, rounds);
    }
    
    stuff = sb_getc();
    if (stuff != E0F)
        return ERR_NO_EOF;
    
    //checksum stuff
    sb_putc(CHK);
    if (handshake() != EOK)
        return EBUSY;
    
    u32 recv_chksum;
    sb_getN((u8*) &recv_chksum, 0x4);
    
    chksum = gen_chksum(dest2, destsize);
    if (chksum != recv_chksum)
        return ERR_BAD_CKSUM;
    
    return 0;
}

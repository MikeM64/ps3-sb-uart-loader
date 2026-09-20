#include "types.h"
#include "dma.h"
#include "sb_mmio_defs.h"
#include "sb_uart.h"

static u64 _af0; //0x00AF0
static u8 _af0_init = 0; //0x00AE0
static u64 _init_af0() //0x187A0
{
	if(_af0_init != 0)
		return (u64)&_af0;
	_af0 = 0;
	_af0_init = 1;
	return (u64)&_af0;
}

static void sb_put_init_seq(u64 *mmio_base, u64 sb_mmio_base, u32 r5, u32 r6, u32 r7, u32 r8, u32 r9)
{
	u32 r4;
	
	//Init MMIO base address.
	*mmio_base = sb_mmio_base;
	
	_dma_put_u32(SB_MMIO_REG(*mmio_base, SB_MMIO_FFF310), 0x808F);
	
	_dma_put_u32(SB_MMIO_REG(*mmio_base, SB_MMIO_FFF310), 0x8E);
	
	if(r6 & 1)
		r4 = 0x4030;
	else
		r4 = 0x4020;
	if(r6 >> 1 != 0)
		r4 |= 8;
	if(r7 != 0)
		r4 |= 4;
	if(r9 != 0)
		r4 |= 3;
	_dma_put_u32(SB_MMIO_REG(*mmio_base, SB_MMIO_FFF300), r4);
	
	_dma_put_u32(SB_MMIO_REG(*mmio_base, SB_MMIO_FFF304), 0x00);
	
	r4 = 2;
	if(r8 != 0)
		r4 |= 0x1000;
	if(r8 != 0)
		r4 |= 0x800;
	_dma_put_u32(SB_MMIO_REG(*mmio_base, SB_MMIO_FFF314), r4);
	
	_dma_put_u32(SB_MMIO_REG(*mmio_base, SB_MMIO_FFF318), r5);
}

// No need to call sb_init() if it was done previously on lv1ldr
void sb_init(void)
{
	sb_put_init_seq((u64*)_init_af0(), SB_MMIO_BASE, 0x17, 0, 0, 0, 0);
}

// Return the number of bytes waiting in RX queue
int sb_getRXqueue(void)
{
	return _dma_get_u32(SB_MMIO_REG(SB_MMIO_BASE, SB_MMIO_STAT_FFF308)) & 0x1F;
}

// Write a byte in the TX queue
void sb_putc(u32 c)
{
	while(!(_dma_get_u32(SB_MMIO_REG(SB_MMIO_BASE, SB_MMIO_STAT_FFF308)) & SB_STAT_FFF308_CAN_PUTC));
	_dma_put_u32(SB_MMIO_REG(SB_MMIO_BASE, SB_MMIO_PUTCHAR), c);
}

void sb_drainRXqueue(void)
{
    while (sb_getRXqueue()) {
        (void)sb_getc();    
    }
}

// Read 1 byte from RX queue
// Use wisely, this is blocking
u8 sb_getc(void)
{
	while(!(_dma_get_u32(SB_MMIO_REG(SB_MMIO_BASE, SB_MMIO_STAT_FFF308)) & 0x1F));
	return _dma_get_u32(SB_MMIO_REG(SB_MMIO_BASE, SB_MMIO_FFF320)) & 0xFF;
}

// Read N bytes from RX queue
// RX queue should be empty befure using this one
int sb_getN(u8 *ptr, u32 len)
{
    if (len > 0x10)
	return -1;
    
    while (len--)
	*ptr++ = sb_getc();
    
    return 0;
}

int sb_putN(u8 *ptr, u32 len)
{
    while(len--)
        sb_putc(*ptr++);
    
    return 0;
}


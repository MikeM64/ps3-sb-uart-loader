/*
 * sb_uart.h
 *
 * Copyright (C) 2026 MikeM64
 *
 * Licensed under the terms of GNU GPL, version 2
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 */

#ifndef __SB_UART_H__
#define __SB_UART_H__

#include "types.h"

void sb_init(void);
int sb_getRXqueue(void);
void sb_putc(u32 c);
void sb_drainRXqueue(void);
u8 sb_getc(void);
int sb_getN(u8 *ptr, u32 len);
int sb_putN(u8 *ptr, u32 len);

#endif /* __SB_UART_H__ */


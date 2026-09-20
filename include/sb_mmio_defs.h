/*
 * sb_mmio_defs.h
 *
 * Copyright (C) 2026 MikeM64
 *
 * Licensed under the terms of GNU GPL, version 2
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 */

#ifndef __SB_MMIO_DEFS_H__
#define __SB_MMIO_DEFS_H__

/*! SB MMIO base address. */
#define SB_MMIO_BASE 0x24000000000

/*! SB MMIO regs. */
#define SB_MMIO_FFF310 0xFFF310
#define SB_MMIO_FFF300 0xFFF300
#define SB_MMIO_FFF304 0xFFF304
#define SB_MMIO_FFF314 0xFFF314
#define SB_MMIO_FFF318 0xFFF318
#define SB_MMIO_FFF320 0xFFF320

#define SB_MMIO_STAT_FFF308 0xFFF308
#define SB_MMIO_PUTCHAR 0xFFF31C

/*! Status bits. */
#define SB_STAT_FFF308_CAN_PUTC 0x100

/*! MMIO reg access. */
#define SB_MMIO_REG(base, offset) ((base)+(offset))

#endif /* __SB_MMIO_DEFS_H__ */

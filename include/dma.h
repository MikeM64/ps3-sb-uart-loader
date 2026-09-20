/*
 * dma.h
 *
 * Copyright (C) 2026 MikeM64
 *
 * Licensed under the terms of GNU GPL, version 2
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 */

#ifndef __DMA_H__
#define __DMA_H__

#include "types.h"
#include "ppc_intrinsics.h"

#define _dma_put_u32(address, data) \
{ \
    *((u32 *)address) = data; \
    _EIEIO(); \
    _SYNC(); \
}


#define _dma_get_u32(ppu_address) \
    *((u32 *)ppu_address)

#endif /* __DMA_H__ */


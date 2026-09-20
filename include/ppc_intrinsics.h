/*
 * ppc_intrinsics.h
 *
 * Copyright (C) 2026 MikeM64
 *
 * Licensed under the terms of GNU GPL, version 2
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 */

#ifndef __PPC_INTRINSICS_H__
#define __PPC_INTRINSICS_H__

#define _EIEIO() \
    asm volatile ("eieio" : : : "memory")
#define _SYNC() \
    asm volatile ("sync" : : : "memory")

#endif /* __PPC_INTRINSICS_H__ */


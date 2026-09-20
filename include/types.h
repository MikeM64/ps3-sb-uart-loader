/*
 * types.h
 *
 * Copyright (C) 2026 MikeM64
 *
 * Licensed under the terms of GNU GPL, version 2
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 */

#ifndef __TYPES_H__
#define __TYPES_H__

typedef char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef long long s64;
typedef unsigned long long u64;
typedef unsigned long size_t;
typedef unsigned long off_t;
typedef unsigned int in_addr_t;

#define EOK 0
#define EBUSY 16
#define ERR_NO_EOF 20
#define ERR_BAD_CKSUM 21

#define NULL ((void *)0)

#endif /* __TYPES_H__ */


#ifndef _MEMCPY_H_
#define _MEMCPY_H_

#include "types.h"

void* memcpy(void *dst, const void *src, int size);
s32 memcmp(const void *ptr1, const void *ptr2, u32 n);
s32 strcmp(const char *ptr1, const char *ptr2);
s32 strncmp(const char *ptr1, const char *ptr2, u32 n);
void* memset ( void *dst, int val, int size );
u32 strlen(const char *str);
u32 strncpy(char *dst, const char *src, u32 dst_len);

#endif

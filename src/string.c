/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "types.h"
#include "string.h"

void*
memcpy ( void *dst, const void *src, int size )
{
  unsigned long long *ret_ptr = (unsigned long long*)dst ;

__asm__ __volatile__("mr %%r3, %1\n\t"           /* *dst */
                     "mr %%r4, %2\n\t"           /* *src */
                     "mr %%r5, %3\n\t"           /* size */
                     "cmpdi %%r5, 0\n\t"         /* size is non-zero? */
                     "beq done\n\t"              /* nothing to do */

                     "mtctr %%r5\n\t"            /* init counter reg */
                     "li %%r7, 0\n\t"            /* counter */

                     "copy:\n\t"
		     
                     "lbzx %%r6, %%r4, %%r7\n\t" /* load byte */
		     "stbx %%r6, %%r3, %%r7\n\t" /* store byte */
                     "addi %%r7, %%r7, 1\n\t"    /* increment counter */
		     
                     "bdnz copy\n\t"             /* loop */
                     "check:\n\t"
                     "cmpd %%r5, %%r7\n\t"       /* verify full copy */
                     "beq  done\n\t"
                     "li   %%r3, -1\n\t"         /* error */
                     "done:\n\t"
                     "mr   %0, %%r3\n\t"
                    :"=r"(ret_ptr)
                    :"r"(dst), "r"(src), "r"(size)
                    :"r0", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "lr", "ctr", "xer", "cr0", "cr1", "cr5", "cr6", "cr7", "memory"
                    );
  return ( (void*)ret_ptr ) ;
}

s32 memcmp(const void *ptr1, const void *ptr2, u32 n)
{
    s32 res = 0;
    const u8 *p1 = ptr1, *p2 = ptr2;
  
    while(n-- > 0 && res == 0)
	res = *p1++ - *p2++;
  
    return res;
}

s32 strcmp(const char *ptr1, const char *ptr2)
{
    s32 res = 0;
    const char *p1 = ptr1, *p2 = ptr2;

    while(res == 0 && *p1 != '\0' && *p2 != '\0')
        res = *p1++ - *p2++;

    return res;
}

s32 strncmp(const char *ptr1, const char *ptr2, u32 n)
{
    s32 res = 0;
    const char *p1 = ptr1, *p2 = ptr2;

    while(n-- > 0 && res == 0 && *p1 != '\0' && *p2 != '\0')
        res = *p1++ - *p2++;

    return res;
}

void*
memset ( void *dst, int val, int size )
{
  unsigned long long *ret_ptr = (unsigned long long*)dst ;

__asm__ __volatile__("mr %%r3, %1\n\t"            /* dst */
                     "mr %%r4, %2\n\t"            /* src */
                     "mr %%r5, %3\n\t"            /* size */
                     "cmpdi %%r5, 0\n\t"          /* size is non-zero? */
                     "beq   done2\n\t"             /* nothing to do */

                     "mtctr %%r5\n\t"             /* init counter reg */
                     "li    %%r7, 0\n\t"          /* counter */

                     "set:\n\t"
                     "stbx  %%r4, %%r3, %%r7\n\t" /* store byte */
                     "addi  %%r7, %%r7, 1\n\t"    /* increment counter */
		 "eieio\n\t"				  /* eieio - block */
                     "bdnz  set\n\t"              /* loop */

                     "check2:\n\t"
                     "cmpd  %%r5, %%r7\n\t"       /* verify size */
                     "beq done2\n\t"
                     "li    %%r3, -1\n\t"         /* error */

                     "done2:\n\t"
                     "mr    %0, %%r3\n\t"
                    :"=r"(ret_ptr)
                    :"r"(dst), "r"(val), "r"(size)
                    :"r0", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "lr", "ctr", "xer", "cr0", "cr1", "cr5", "cr6", "cr7", "memory"
                    );
  return ( (void*)ret_ptr ) ;
}

u32 strlen(const char *str)
{
    if (!str)
        return 0;

    u32 i = 0;
    u8 *c = (u8*)str;
    while(*c++) i++;

    return i;
}

u32 strncpy(char *dst, const char *src, u32 dst_len)
{
    u32  i = 0;
    u8  *c = (u8 *)src;

    if (!dst || !src) {
        return 0;
    }

    while(i < dst_len && *c) {
        dst[i++] = *c++;
    }

    return i;
}

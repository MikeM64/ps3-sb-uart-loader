/*!
* compiler.h - Compiler specific definitions.
* 
* Copyright (C) 2012 naehrwert
* 
* Licensed under the terms of the GNU GPL, version 2
* http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*/

#ifndef _COMPILER_H_
#define _COMPILER_H_

/*! Inline assembler. */
#define _ASM asm
#define _ASMV asm volatile

/*! Attributes. */
#define _ALIGNED(v) __attribute__((aligned(v)))
#define _NORETURN __attribute__((noreturn))
#define _PACKED __attribute__((packed))
#define _SECTION(n) __attribute__ ((section(n)))

/*! Expected condition. */
#define _EXPECTED(e) __builtin_expect((e), 1)
/*! Unexpected condition. */
#define _UNEXPECTED(e) __builtin_expect((e), 0)

#endif


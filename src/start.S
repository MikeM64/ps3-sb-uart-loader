#include "assembly.h"

.text
.section .start,"ax",@progbits
.align 3

.global _start

_start:

_hang:
	b _hang
	/* Set up the TOC register. */
	lis r2, (__toc_start+0x8000)@h
	ori r2, r2, (__toc_start+0x8000)@l

	/* Set up the stack. */
	lis r1, __stack_bottom@h
	ori r1, r1, __stack_bottom@l
	
	/* Make space for the first stack frame. */
	addi r1, r1, 64
	li r0, 0
	std r0, 0(r1)

	/* Clear BSS. */
	lis r3, __bss_start@h
	ori r3, r3, __bss_start@l
	lis r4, __bss_end@h
	ori r4, r4, __bss_end@l
	li r5, 0
_bss_loop:
	std r5, 0(r3)
	addi r3, r3, 8
	cmpld r3, r4
	blt _bss_loop
	
	/* Jump to main. */
	bl main
	
	/*! Hang if main returns. */
	b _hang

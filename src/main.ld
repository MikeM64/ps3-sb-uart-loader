OUTPUT_FORMAT("elf64-powerpc")
OUTPUT_ARCH(powerpc:common64)

/* Base is at 0. */
__base = 0;
/* Execution starts at 0x100. */
__start_base = 0x100;
/** PPC Interrupts **/
__machine_check = 0x200;
__data_storage = 0x300;
__instruction_storage = 0x400;
__external = 0x500;
__alignment = 0x600;
__program = 0x700;
__fp = 0x800;
__decrementer = 0x900;
/* Return from LV0 execution handler */
__lvz_ret = 0x5000;

/* lvx start. */
__lvx_base = 0x20000;

ENTRY(_start)

PHDRS {
        start       PT_LOAD FLAGS(5);
        text        PT_LOAD FLAGS(5);
	rodata      PT_LOAD FLAGS(4);
	data        PT_LOAD FLAGS(6);
	bss         PT_LOAD FLAGS(6);
        /*
        align_int   PT_LOAD FLAGS(5);
        ext_int     PT_LOAD FLAGS(5);
        lvzret      PT_LOAD FLAGS(5);
        */
}

SECTIONS {
	. = __base;
	/* Fill first 0x100 bytes with zero. */
	.start : {
		. = __start_base;
		*(.start)
	} :start = 0

	. = __lvx_base;
	.text : {
		*(.text)
		*(.text.*)
	} :text = 0

	.opd : ALIGN(64) {
            __toc_start = .;
	    *(.opd)
	} :rodata = 0
	
	.rodata : {
		*(.rodata)
		*(.rodata.*)
	} : rodata

	.data : {
		*(.data)
		*(.data.*)
		*(.sdata)
		*(.sdata.*)
	} :data = 0
	


        /*
	. = __external;
	.ext_int : {
		. = __external;
		*(.ext_int)
	} :ext_int = 0
        */
        /*
	. = __alignment;
	.align_int : {
		. = __alignment;
		*(.align_int)
	} :align_int = 0
        */
        /*
        . = __lvz_ret;
        .lvzret : {
                *(.lvzret)
                *(.lvzret.*)
        } :lvzret = 0
        */
	
	.bss : ALIGN(64) {
	    __bss_start = .;
                *(.dynbss)
                *(.gnu.linkonce.b.*)
		*(COMMON)
		*(.bss)
		*(.bss.*)
		*(.sbss)
		*(.sbss.*)
	    __bss_end = .;
	} :bss = 0
	
	. = ALIGN(64);
	.stack (NOLOAD) : {
		__stack_top = .;
		. += 0x10000;
		__stack_bottom = .;
	}
	
	. = ALIGN(64);
	. += 0x1000;			/* temp fix to avoid a stack overflow to mess heap struct */
	__free_memory = .;
        
}

OUTPUT_FORMAT("binary")
OUTPUT_ARCH("powerpc:common64")

__base = 0x2401FCFF1000;

ENTRY(_start)

SECTIONS
{
    . = __base;
    .start : {
        *(.start)
        . = ALIGN(8);
    }

    .text : {
        code = .;
        *(.text)
        *(.text.*)
        . = ALIGN(8);
    }

    .opd : ALIGN(8) {
        __toc_start = .;
        *(.opd)
    }

    .rodata : ALIGN(8) {
        *(.rodata)
        *(.rodata.*)
    }

    .bss : ALIGN(8) {
        __bss_start = .;
        *(.bss)
        *(.bss.*)
        __bss_end = .;
    }

    __stack_top = 0x20000;
    . += 0x10000;
    __stack_bottom = .;

    . = ALIGN(64);
    . += 0x1000;			/* temp fix to avoid a stack overflow to mess heap struct */
    __free_memory = .;
}

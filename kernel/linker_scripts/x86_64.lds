OUTPUT_FORMAT(elf64-x86-64)
ENTRY(kmain)

PHDRS
{
    limine_requests PT_LOAD FLAGS(0x06);
    text PT_LOAD FLAGS(0x05);
    rodata PT_LOAD FLAGS(0x04);
    data PT_LOAD FLAGS(0x06);
}

SECTIONS
{
    . = 0xffffffff80000000;

    .limine_requests : {
        KEEP(*(.limine_requests_start))
        KEEP(*(.limine_requests))
        KEEP(*(.limine_requests_end))
    } :limine_requests

    . = ALIGN(CONSTANT(MAXPAGESIZE));

    .text : {
        *(.text .text.*)
    } :text

    . = ALIGN(CONSTANT(MAXPAGESIZE));

    .rodata : {
        *(.rodata .rodata.*)
    } :rodata

    .note.gnu.build-id : {
        *(.note.gnu.build-id)
    } :rodata

    . = ALIGN(CONSTANT(MAXPAGESIZE));

    .init_array : {
        __init_array_start = .;
        KEEP(*(SORT(.init_array.?)))
        KEEP(*(SORT(.init_array.??)))
        KEEP(*(SORT(.init_array.???)))
        KEEP(*(SORT(.init_array.????)))
        KEEP(*(SORT(.init_array.?????)))
        KEEP(*(.init_array.*))
        KEEP(*(.init_array))
        __init_array_end = .;
    } :data

    .ctors : {
        __ctors_start = .;
        KEEP(*(.ctors))
        KEEP(*(SORT(.ctors.*)))
        __ctors_end = .;
    } :data

    .data : {
        *(.data .data.*)
    } :data

    .bss : {
        *(.bss .bss.*)
        *(COMMON)
    } :data

    /DISCARD/ : {
        *(.eh_frame*)
        *(.note .note.*)
        *(.got .got.*)
    }
}
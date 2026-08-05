/* ============================================================================
 * Kernec Kernel Linker Script
 * ============================================================================
 *
 * Architecture : x86_64
 * Kernel Model : Higher-Half Kernel
 *
 * Responsibilities:
 *   - Define the kernel entry point.
 *   - Define the kernel memory layout.
 *   - Preserve Limine request structures.
 *   - Export useful linker symbols.
 *
 * Engineering Principle:
 *   Keep the linker script as small as possible. Add new sections only when
 *   the kernel actually requires them.
 * ============================================================================
 */

OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)
ENTRY(_start)

/* Higher-half kernel virtual base */
KERNEL_BASE = 0xffffffff80000000;

SECTIONS
{
    . = KERNEL_BASE + SIZEOF_HEADERS;

    /* --------------------------------------------------------------------- */
    /* Kernel begins                                                         */
    /* --------------------------------------------------------------------- */

    _kernel_start = .;

    /* --------------------------------------------------------------------- */
    /* Executable code                                                       */
    /* --------------------------------------------------------------------- */

    .text : AT(ADDR(.text) - KERNEL_BASE)
    {
        *(.text)
        *(.text.*)
    }

    /* --------------------------------------------------------------------- */
    /* Read-only data                                                        */
    /* --------------------------------------------------------------------- */

    .rodata : AT(ADDR(.rodata) - KERNEL_BASE)
    {
        *(.rodata)
        *(.rodata.*)
    }

    /* --------------------------------------------------------------------- */
    /* Writable data                                                         */
    /* --------------------------------------------------------------------- */

    .data : AT(ADDR(.data) - KERNEL_BASE)
    {
        *(.data)
        *(.data.*)
    }

    /* --------------------------------------------------------------------- */
    /* Limine boot protocol requests                                         */
    /* --------------------------------------------------------------------- */

    .limine_requests : AT(ADDR(.limine_requests) - KERNEL_BASE)
    {
        . = ALIGN(8);
        KEEP(*(.limine_requests))
        . = ALIGN(8);
    }

    /* --------------------------------------------------------------------- */
    /* Zero-initialized memory                                               */
    /* --------------------------------------------------------------------- */

    .bss (NOLOAD) : AT(ADDR(.bss) - KERNEL_BASE)
    {
        . = ALIGN(16);

        _bss_start = .;

        *(.bss)
        *(.bss.*)
        *(COMMON)

        _bss_end = .;
    }

    /* --------------------------------------------------------------------- */
    /* Kernel ends                                                           */
    /* --------------------------------------------------------------------- */

    _kernel_end = .;

    /* Discard compiler metadata we don't need. */

    /DISCARD/ :
    {
        *(.comment)
    }
}

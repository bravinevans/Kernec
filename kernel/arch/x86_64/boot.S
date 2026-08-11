/*
 * File:
 * kernel/arch/x86_64/boot.S
 *
 * Responsibility:
 * First executable code of the Kernec kernel.
 *
 * Dependencies:
 * kmain()
 *
 * Notes:
 * This file performs the absolute minimum work required to
 * establish a valid execution environment before transferring
 * control to the C kernel.
 */

.global _start
.extern kmain

.type _start, @function

/*
 * Minimal Limine protocol section.
 *
 * For Sprint 3 we only advertise the base revision and terminate
 * the request list. More requests (framebuffer, memory map, modules,
 * etc.) will be added in later sprints.
 */
.section .limine_requests, "aw", @progbits
.align 8

/* Limine requests start marker */
.quad 0xf6b8f4b39de7d1ae
.quad 0xfab91a6940fcb9cf
.quad 0x785c6ed015d3e316
.quad 0x181e920a7852b9d9

/* Base revision request */
.quad 0xf9562b2d5c95a6c8
.quad 0x6a7b384944536bdc
.quad 3
.quad 0

/* Limine requests end marker */
.quad 0xadc0e0531bb10d03
.quad 0x9572709f31764c62

.text

_start:
    /* Disable interrupts. */
    cli

    /* Initialize the kernel stack. */
    lea stack_top(%rip), %rsp

    /* Align the stack to the SysV AMD64 ABI. */
    andq $-16, %rsp

    /* Enter the C kernel. */
    call kmain

/*
 * kmain() should never return.
 */
.hang:
    cli
    hlt
    jmp .hang

.bss
.align 16

stack_bottom:
    .skip 65536          /* 64 KiB */

stack_top:

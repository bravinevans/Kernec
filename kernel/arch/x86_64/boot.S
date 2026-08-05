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

.quad 0xf9562b2d5c95a6c8
.quad 0x6a78a343b4d6d6b6
.quad 3

.quad 0

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

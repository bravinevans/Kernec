/*
 * Kernec Operating System
 *
 * File: entry.S
 * Architecture: x86_64
 * Subsystem: System Calls
 *
 * First assembly entry point for SYSCALL.
 */

.text

.intel_syntax noprefix

.global syscall_entry
.type syscall_entry, @function

.extern syscall_dispatch

syscall_entry:

    /*
     * The SYSCALL instruction stores the fourth argument
     * in R10. The System V AMD64 ABI expects it in RCX.
     */
    mov rcx, r10

    /*
     * Temporary return.
     *
     * Sprint 5 will replace this with a call into
     * syscall_dispatch().
     */
    ret

.size syscall_entry, .-syscall_entry

.att_syntax prefix

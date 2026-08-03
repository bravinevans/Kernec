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

.global syscall_entry
.type syscall_entry, @function

.extern syscall_dispatch

syscall_entry:

    /*
     * Save the original fourth syscall argument.
     *
     * The SYSCALL instruction places it in R10.
     * The C ABI expects the fourth argument in RCX.
     */
    mov rcx, r10

    /*
     * Temporary return.
     *
     * Later this function will invoke
     * syscall_dispatch().
     */
    ret

.size syscall_entry, .-syscall_entry

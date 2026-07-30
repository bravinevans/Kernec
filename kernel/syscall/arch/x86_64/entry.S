/*
 * Kernec Operating System
 *
 * File: entry.S
 * Subsystem: System Call Interface
 * Purpose: x86_64 system call entry stub.
 *
 * SPDX-License-Identifier: MIT
 */

.text

.global syscall_entry

syscall_entry:

    /*
     * TODO:
     * Save registers.
     * Build stack frame.
     * Call syscall_dispatch().
     * Restore registers.
     * Return using SYSRET.
     */

    ret

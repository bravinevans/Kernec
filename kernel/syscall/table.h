/*
 * Kernec Operating System
 *
 * File: table.h
 * Subsystem: System Call Interface
 * Purpose: Defines the system call table interface.
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef KERNEC_SYSCALL_TABLE_H
#define KERNEC_SYSCALL_TABLE_H

#include <stdint.h>

/*
 * Generic system call handler prototype.
 *
 * Every system call implementation must match this signature.
 */
typedef uint64_t (*syscall_handler_t)(
    uint64_t arg1,
    uint64_t arg2,
    uint64_t arg3,
    uint64_t arg4,
    uint64_t arg5,
    uint64_t arg6
);

/*
 * System call table.
 */
extern syscall_handler_t syscall_table[];

#endif /* KERNEC_SYSCALL_TABLE_H */

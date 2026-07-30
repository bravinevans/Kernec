/*
 * Kernec Operating System
 *
 * File: dispatcher.h
 * Subsystem: System Call Interface
 * Purpose: Public interface for the system call dispatcher.
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef KERNEC_SYSCALL_DISPATCHER_H
#define KERNEC_SYSCALL_DISPATCHER_H

#include <stdint.h>

/*
 * Dispatches a system call to the appropriate kernel handler.
 *
 * Parameters:
 *   number - System call number.
 *   arg1..arg6 - System call arguments.
 *
 * Returns:
 *   System call return value.
 */
uint64_t syscall_dispatch(
    uint64_t number,
    uint64_t arg1,
    uint64_t arg2,
    uint64_t arg3,
    uint64_t arg4,
    uint64_t arg5,
    uint64_t arg6
);

#endif /* KERNEC_SYSCALL_DISPATCHER_H */

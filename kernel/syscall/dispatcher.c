/*
 * Kernec Operating System
 *
 * File: dispatcher.c
 * Subsystem: System Call Interface
 * Purpose: Dispatches system calls to registered handlers.
 *
 * SPDX-License-Identifier: MIT
 */

#include "dispatcher.h"
#include "table.h"
#include "../../include/kernec/syscall.h"

uint64_t syscall_dispatch(
    uint64_t number,
    uint64_t arg1,
    uint64_t arg2,
    uint64_t arg3,
    uint64_t arg4,
    uint64_t arg5,
    uint64_t arg6
)
{
    /*
     * Reject invalid system call numbers.
     */
    if (number >= SYSCALL_COUNT)
        return (uint64_t)-1;

    return syscall_table[number](
        arg1,
        arg2,
        arg3,
        arg4,
        arg5,
        arg6
    );
}

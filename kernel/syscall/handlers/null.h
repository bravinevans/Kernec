/*
 * Kernec Operating System
 *
 * File: null.h
 * Subsystem: System Call Interface
 * Purpose: Declaration of the null system call handler.
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef KERNEC_SYSCALL_NULL_H
#define KERNEC_SYSCALL_NULL_H

#include <stdint.h>

uint64_t sys_null(
    uint64_t arg1,
    uint64_t arg2,
    uint64_t arg3,
    uint64_t arg4,
    uint64_t arg5,
    uint64_t arg6
);

#endif /* KERNEC_SYSCALL_NULL_H */

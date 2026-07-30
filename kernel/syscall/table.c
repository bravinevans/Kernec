/*
 * Kernec Operating System
 *
 * File: table.c
 * Subsystem: System Call Interface
 * Purpose: Defines the kernel system call table.
 *
 * SPDX-License-Identifier: MIT
 */

#include "table.h"

/*
 * Forward declarations of system call handlers.
 * Additional handlers will be added here as Kernec grows.
 */
#include "handlers/null.h"

/*
 * Global system call table.
 *
 * The index into this array is the system call number.
 */
syscall_handler_t syscall_table[] = {
    sys_null,   /* System Call 0 */
};

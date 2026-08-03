/*
 * Kernec Operating System
 *
 * File: entry.h
 * Architecture: x86_64
 * Subsystem: System Call Interface
 *
 * Assembly entry point declarations.
 */

#ifndef KERNEC_X86_64_SYSCALL_ENTRY_H
#define KERNEC_X86_64_SYSCALL_ENTRY_H

#ifdef __cplusplus
extern "C" {
    #endif

    void syscall_entry(void);

    #ifdef __cplusplus
}
#endif

#endif /* KERNEC_X86_64_SYSCALL_ENTRY_H */

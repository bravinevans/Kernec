/*
 * File:
 * kernel/arch/x86_64/cpu.c
 *
 * Responsibility:
 * x86_64 CPU utility functions.
 */

#include <kernec/cpu.h>

void cpu_halt_forever(void)
{
    for (;;) {
        __asm__ volatile ("cli");
        __asm__ volatile ("hlt");
    }
}

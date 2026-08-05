/*
 * File:
 * kernel/core/panic.c
 *
 * Responsibility:
 * Handle unrecoverable kernel errors.
 */

#include <kernec/panic.h>
#include <kernec/serial.h>
#include <kernec/cpu.h>

void panic(const char *reason)
{
    serial_write("\n=== KERNEL PANIC ===\n");

    if (reason != 0) {
        serial_write(reason);
    }

    serial_write("\nSystem halted.\n");

    cpu_halt_forever();
}

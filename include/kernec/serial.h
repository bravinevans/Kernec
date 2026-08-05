/*
 * File:
 * kernel/arch/x86_64/serial.h
 *
 * Responsibility:
 * Public interface for the early COM1 serial driver.
 *
 * Dependencies:
 * None.
 *
 * Notes:
 * This interface is used by the early kernel before any console
 * or terminal subsystem exists.
 */

#ifndef KERNEC_ARCH_X86_64_SERIAL_H
#define KERNEC_ARCH_X86_64_SERIAL_H

void serial_init(void);

void serial_putchar(char c);

void serial_write(const char *string);

#endif /* KERNEC_ARCH_X86_64_SERIAL_H */

/*
 * File:
 * kernel/arch/x86_64/serial.c
 *
 * Responsibility:
 * Minimal COM1 serial driver for early kernel output.
 *
 * Dependencies:
 * serial.h
 *
 * Notes:
 * This driver uses legacy x86 port I/O.
 * It exists solely for early boot diagnostics.
 */

#include <kernec/serial.h>

typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;

#define COM1 0x3F8

#define COM1_DATA (COM1 + 0)
#define COM1_IER  (COM1 + 1)
#define COM1_FCR  (COM1 + 2)
#define COM1_LCR  (COM1 + 3)
#define COM1_MCR  (COM1 + 4)
#define COM1_LSR  (COM1 + 5)

#define COM1_DLL  (COM1 + 0)
#define COM1_DLM  (COM1 + 1)

/*
 * Write one byte to an I/O port.
 */
static inline void outb(uint16_t port, uint8_t value)
{
    __asm__ volatile (
        "outb %0, %1"
        :
        : "a"(value), "Nd"(port)
    );
}

/*
 * Read one byte from an I/O port.
 */
static inline uint8_t inb(uint16_t port)
{
    uint8_t value;

    __asm__ volatile (
        "inb %1, %0"
        : "=a"(value)
        : "Nd"(port)
    );

    return value;
}

/*
 * Wait until COM1 can transmit.
 */
static void serial_wait_tx_ready(void)
{
    while ((inb(COM1_LSR) & 0x20) == 0) {
        /* Busy wait */
    }
}

/*
 * Send one character without translation.
 */
static void serial_raw_putchar(char c)
{
    serial_wait_tx_ready();
    outb(COM1_DATA, (uint8_t)c);
}

void serial_init(void)
{
    outb(COM1_IER, 0x00);

    outb(COM1_LCR, 0x80);

    outb(COM1_DLL, 0x01);
    outb(COM1_DLM, 0x00);

    outb(COM1_LCR, 0x03);

    outb(COM1_FCR, 0xC7);

    outb(COM1_MCR, 0x0B);
}

void serial_putchar(char c)
{
    if (c == '\n') {
        serial_raw_putchar('\r');
    }

    serial_raw_putchar(c);
}

void serial_write(const char *string)
{
    if (string == 0) {
        return;
    }

    while (*string) {
        serial_putchar(*string);
        string++;
    }
}

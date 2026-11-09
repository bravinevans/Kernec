/*
 * File:
 * kernel/arch/x86_64/interrupts.c
 *
 * Responsibility:
 * x86_64 interrupt descriptor table setup and early exception handling.
 */

#include <kernec/cpu.h>
#include <kernec/interrupts.h>
#include <kernec/serial.h>

#include <stdint.h>
#include <stddef.h>

struct idt_entry {
    uint16_t base_low;
    uint16_t selector;
    uint8_t ist;
    uint8_t flags;
    uint16_t base_mid;
    uint32_t base_high;
    uint32_t reserved;
} __attribute__((packed));

struct idtr {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
extern void isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);
extern void isr20(void);
extern void isr21(void);
extern void isr22(void);
extern void isr23(void);
extern void isr24(void);
extern void isr25(void);
extern void isr26(void);
extern void isr27(void);
extern void isr28(void);
extern void isr29(void);
extern void isr30(void);
extern void isr31(void);

static struct idt_entry idt[256];
static struct idtr idtr;

static void idt_set_gate(uint8_t vector, void (*handler)(void), uint8_t flags)
{
    uintptr_t address = (uintptr_t)handler;

    idt[vector].base_low = (uint16_t)(address & 0xFFFFu);
    idt[vector].base_mid = (uint16_t)((address >> 16) & 0xFFFFu);
    idt[vector].base_high = (uint32_t)((address >> 32) & 0xFFFFFFFFu);
    idt[vector].selector = 0x08;
    idt[vector].ist = 0;
    idt[vector].flags = flags;
    idt[vector].reserved = 0;
}

static void serial_write_hex32(uint32_t value)
{
    static const char digits[] = "0123456789ABCDEF";
    char buffer[9];
    int i;

    buffer[8] = '\0';

    for (i = 7; i >= 0; --i) {
        buffer[i] = digits[value & 0xF];
        value >>= 4;
    }

    serial_write("0x");
    serial_write(buffer);
}

void interrupt_handler(unsigned int vector, unsigned long error_code)
{
    serial_write("\n[KERNEC] Unhandled exception.\n");
    serial_write("Vector : ");
    serial_write_hex32((uint32_t)vector);
    serial_write("\n");

    serial_write("Error  : ");
    serial_write_hex32((uint32_t)error_code);
    serial_write("\n");

    serial_write("Kernel halted.\n");
    cpu_halt_forever();
}

void interrupts_init(void)
{
    for (size_t i = 0; i < 256; ++i) {
        idt[i].base_low = 0;
        idt[i].base_mid = 0;
        idt[i].base_high = 0;
        idt[i].selector = 0;
        idt[i].ist = 0;
        idt[i].flags = 0;
        idt[i].reserved = 0;
    }

    idt_set_gate(0, isr0, 0x8E);
    idt_set_gate(1, isr1, 0x8E);
    idt_set_gate(2, isr2, 0x8E);
    idt_set_gate(3, isr3, 0x8E);
    idt_set_gate(4, isr4, 0x8E);
    idt_set_gate(5, isr5, 0x8E);
    idt_set_gate(6, isr6, 0x8E);
    idt_set_gate(7, isr7, 0x8E);
    idt_set_gate(8, isr8, 0x8E);
    idt_set_gate(9, isr9, 0x8E);
    idt_set_gate(10, isr10, 0x8E);
    idt_set_gate(11, isr11, 0x8E);
    idt_set_gate(12, isr12, 0x8E);
    idt_set_gate(13, isr13, 0x8E);
    idt_set_gate(14, isr14, 0x8E);
    idt_set_gate(15, isr15, 0x8E);
    idt_set_gate(16, isr16, 0x8E);
    idt_set_gate(17, isr17, 0x8E);
    idt_set_gate(18, isr18, 0x8E);
    idt_set_gate(19, isr19, 0x8E);
    idt_set_gate(20, isr20, 0x8E);
    idt_set_gate(21, isr21, 0x8E);
    idt_set_gate(22, isr22, 0x8E);
    idt_set_gate(23, isr23, 0x8E);
    idt_set_gate(24, isr24, 0x8E);
    idt_set_gate(25, isr25, 0x8E);
    idt_set_gate(26, isr26, 0x8E);
    idt_set_gate(27, isr27, 0x8E);
    idt_set_gate(28, isr28, 0x8E);
    idt_set_gate(29, isr29, 0x8E);
    idt_set_gate(30, isr30, 0x8E);
    idt_set_gate(31, isr31, 0x8E);

    idtr.limit = (uint16_t)(sizeof(idt) - 1);
    idtr.base = (uint64_t)&idt[0];

    __asm__ volatile("lidt %0" : : "m"(idtr));
}

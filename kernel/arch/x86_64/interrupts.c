/*
 * x86_64 Interrupt Descriptor Table and common interrupt dispatch.
 *
 * Hardware interrupt-controller setup and EOI handling are intentionally
 * separate; this module leaves IF clear and does not enable IRQ delivery.
 */
#include <kernec/cpu.h>
#include <kernec/interrupts.h>
#include <kernec/serial.h>

#include <stddef.h>
#include <stdint.h>

struct idt_entry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t ist;
    uint8_t attributes;
    uint16_t offset_middle;
    uint32_t offset_high;
    uint32_t reserved;
} __attribute__((packed));

struct idtr {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

typedef void (*isr_stub_t)(void);

extern isr_stub_t isr_stub_table[KERNEC_IDT_ENTRIES];

static struct idt_entry idt[KERNEC_IDT_ENTRIES] __attribute__((aligned(16)));
static struct idtr idtr;
static interrupt_handler_t handlers[KERNEC_IDT_ENTRIES];

static void idt_set_gate(uint8_t vector, isr_stub_t stub)
{
    const uintptr_t address = (uintptr_t)stub;

    idt[vector].offset_low = (uint16_t)(address & 0xFFFFu);
    idt[vector].selector = 0x08;
    idt[vector].ist = 0;
    idt[vector].attributes = 0x8E; /* Present, ring 0, 64-bit interrupt gate. */
    idt[vector].offset_middle = (uint16_t)((address >> 16) & 0xFFFFu);
    idt[vector].offset_high = (uint32_t)(address >> 32);
    idt[vector].reserved = 0;
}

static void write_hex64(uint64_t value)
{
    static const char digits[] = "0123456789ABCDEF";
    char text[16];

    for (size_t i = sizeof(text); i > 0; --i) {
        text[i - 1] = digits[value & 0xFu];
        value >>= 4;
    }

    for (size_t i = 0; i < sizeof(text); ++i) {
        serial_putchar(text[i]);
    }
}

static void fatal_interrupt(const struct interrupt_frame *frame)
{
    serial_write("\n[KERNEC] Fatal or unhandled CPU event\nVector: 0x");
    write_hex64(frame->vector);
    serial_write("\nError : 0x");
    write_hex64(frame->error_code);
    serial_write("\nRIP   : 0x");
    write_hex64(frame->rip);
    serial_write("\nCS    : 0x");
    write_hex64(frame->cs);
    serial_write("\nRFLAGS: 0x");
    write_hex64(frame->rflags);
    serial_write("\nKernel halted.\n");
    cpu_halt_forever();
}

void interrupts_init(void)
{
    __asm__ volatile ("cli" ::: "memory");

    for (size_t vector = 0; vector < KERNEC_IDT_ENTRIES; ++vector) {
        handlers[vector] = NULL;
        idt_set_gate((uint8_t)vector, isr_stub_table[vector]);
    }

    idtr.limit = (uint16_t)(sizeof(idt) - 1u);
    idtr.base = (uint64_t)(uintptr_t)&idt[0];
    __asm__ volatile ("lidt %0" : : "m"(idtr) : "memory");
}

int interrupt_register_handler(uint8_t vector, interrupt_handler_t handler)
{
    uint64_t flags;

    if (vector < KERNEC_IRQ_BASE || handler == NULL) {
        return -1;
    }

    __asm__ volatile ("pushfq; popq %0; cli" : "=r"(flags) : : "memory");
    handlers[vector] = handler;
    if ((flags & (1u << 9)) != 0) {
        __asm__ volatile ("sti" ::: "memory");
    }

    return 0;
}

void interrupt_dispatch(struct interrupt_frame *frame)
{
    if (frame == NULL || frame->vector >= KERNEC_IDT_ENTRIES) {
        cpu_halt_forever();
    }

    if (frame->vector < KERNEC_EXCEPTION_COUNT) {
        fatal_interrupt(frame);
    }

    interrupt_handler_t handler = handlers[frame->vector];
    if (handler == NULL) {
        fatal_interrupt(frame);
    }

    handler(frame);
}

/* Minimal legacy 8259 PIC support; all lines begin masked. */
#include <kernec/pic.h>

#define PIC1_COMMAND 0x20u
#define PIC1_DATA    0x21u
#define PIC2_COMMAND 0xA0u
#define PIC2_DATA    0xA1u
#define PIC_EOI      0x20u
#define PIC1_OFFSET  0x20u
#define PIC2_OFFSET  0x28u

static inline void outb(uint16_t port, uint8_t value)
{
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t value;
    __asm__ volatile ("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

static inline void io_wait(void)
{
    outb(0x80u, 0);
}

void pic_init(void)
{
    const uint8_t master_mask = inb(PIC1_DATA);
    const uint8_t slave_mask = inb(PIC2_DATA);
    (void)master_mask;
    (void)slave_mask;

    outb(PIC1_DATA, 0xFFu);
    outb(PIC2_DATA, 0xFFu);

    outb(PIC1_COMMAND, 0x11u); io_wait();
    outb(PIC2_COMMAND, 0x11u); io_wait();
    outb(PIC1_DATA, PIC1_OFFSET); io_wait();
    outb(PIC2_DATA, PIC2_OFFSET); io_wait();
    outb(PIC1_DATA, 0x04u); io_wait();
    outb(PIC2_DATA, 0x02u); io_wait();
    outb(PIC1_DATA, 0x01u); io_wait();
    outb(PIC2_DATA, 0x01u); io_wait();

    /* Keep every line masked until a driver explicitly enables it. */
    outb(PIC1_DATA, 0xFFu);
    outb(PIC2_DATA, 0xFFu);
}

int pic_unmask_irq(uint8_t irq)
{
    if (irq >= 16u) {
        return -1;
    }

    if (irq < 8u) {
        uint8_t mask = inb(PIC1_DATA);
        mask = (uint8_t)(mask & (uint8_t)~(1u << irq));
        outb(PIC1_DATA, mask);
        return 0;
    }

    uint8_t slave_mask = inb(PIC2_DATA);
    slave_mask = (uint8_t)(slave_mask & (uint8_t)~(1u << (irq - 8u)));
    outb(PIC2_DATA, slave_mask);

    uint8_t master_mask = inb(PIC1_DATA);
    master_mask = (uint8_t)(master_mask & (uint8_t)~(1u << 2));
    outb(PIC1_DATA, master_mask);
    return 0;
}

void pic_send_eoi(uint8_t vector)
{
    if (vector >= PIC2_OFFSET && vector < PIC2_OFFSET + 8u) {
        outb(PIC2_COMMAND, PIC_EOI);
    }
    if (vector >= PIC1_OFFSET && vector < PIC1_OFFSET + 16u) {
        outb(PIC1_COMMAND, PIC_EOI);
    }
}

/* PIT channel 0 system tick, initially configured for 1000 Hz. */
#include <kernec/interrupts.h>
#include <kernec/timer.h>

#include <stdint.h>

#define PIT_CHANNEL0 0x40u
#define PIT_COMMAND  0x43u
#define PIT_BASE_HZ  1193182u
#define PIT_IRQ_VECTOR 32u

static volatile uint64_t ticks;

static inline void outb(uint16_t port, uint8_t value)
{
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

static void pit_interrupt(struct interrupt_frame *frame)
{
    (void)frame;
    __atomic_add_fetch(&ticks, 1u, __ATOMIC_RELAXED);
}

int timer_init(uint32_t frequency_hz)
{
    if (frequency_hz < 19u || frequency_hz > PIT_BASE_HZ) {
        return -1;
    }

    const uint32_t divisor = PIT_BASE_HZ / frequency_hz;
    if (interrupt_register_handler(PIT_IRQ_VECTOR, pit_interrupt) != 0) {
        return -1;
    }

    outb(PIT_COMMAND, 0x36u); /* Channel 0, lobyte/hibyte, square wave. */
    outb(PIT_CHANNEL0, (uint8_t)(divisor & 0xFFu));
    outb(PIT_CHANNEL0, (uint8_t)((divisor >> 8) & 0xFFu));
    return 0;
}

uint64_t timer_ticks(void)
{
    return __atomic_load_n(&ticks, __ATOMIC_RELAXED);
}

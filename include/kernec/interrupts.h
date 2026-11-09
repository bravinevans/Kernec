/*
 * Public x86_64 interrupt interface.
 *
 * IRQ handlers run in interrupt context with maskable interrupts disabled.
 * They must be bounded and must not block, sleep, or allocate memory.
 */
#ifndef KERNEC_INTERRUPTS_H
#define KERNEC_INTERRUPTS_H

#include <stdint.h>

#define KERNEC_IDT_ENTRIES 256u
#define KERNEC_EXCEPTION_COUNT 32u
#define KERNEC_IRQ_BASE 32u

struct interrupt_frame {
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rbp;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;
    uint64_t vector;
    uint64_t error_code;
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    /* rsp and ss are present when the CPU changed privilege level. */
};

typedef void (*interrupt_handler_t)(struct interrupt_frame *frame);

/* Install and load the x86_64 IDT. This does not enable interrupts. */
void interrupts_init(void);

/* Register a handler for vectors 32-255. Returns 0 on success, -1 on error. */
int interrupt_register_handler(uint8_t vector, interrupt_handler_t handler);

/* Called only by the architecture assembly entry stubs. */
void interrupt_dispatch(struct interrupt_frame *frame);

#endif /* KERNEC_INTERRUPTS_H */

/* Public x86_64 interrupt interface. */
#ifndef KERNEC_INTERRUPTS_H
#define KERNEC_INTERRUPTS_H

#include <stdint.h>

#define KERNEC_IDT_ENTRIES 256u
#define KERNEC_EXCEPTION_COUNT 32u
#define KERNEC_IRQ_BASE 32u

/*
 * General-purpose registers and the normalized vector/error fields are always
 * present. rsp/ss are available only when the CPU entered from another CPL.
 */
struct interrupt_frame {
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
    uint64_t rsi, rdi, rbp, rdx, rcx, rbx, rax;
    uint64_t vector, error_code;
    uint64_t rip, cs, rflags;
};

typedef void (*interrupt_handler_t)(struct interrupt_frame *frame);

/* Install and load the 256-entry IDT. This leaves IF disabled. */
void interrupts_init(void);

/* Register an external-interrupt handler for vectors 32-255. */
int interrupt_register_handler(uint8_t vector, interrupt_handler_t handler);

/* Architecture entry point; called only by assembly stubs. */
void interrupt_dispatch(struct interrupt_frame *frame);

#endif /* KERNEC_INTERRUPTS_H */

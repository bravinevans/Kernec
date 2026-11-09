/* x86_64 CPU utility functions. */
#include <kernec/cpu.h>

void cpu_halt_forever(void)
{
    for (;;) {
        __asm__ volatile ("cli; hlt" ::: "memory");
    }
}

void cpu_enable_interrupts(void)
{
    __asm__ volatile ("sti" ::: "memory");
}

void cpu_idle_forever(void)
{
    for (;;) {
        /* STI's interrupt shadow makes the following HLT race-free. */
        __asm__ volatile ("sti; hlt" ::: "memory");
    }
}

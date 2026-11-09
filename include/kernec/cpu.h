/* x86_64 CPU utilities used by early kernel code. */
#ifndef KERNEC_CPU_H
#define KERNEC_CPU_H

void cpu_halt_forever(void);
void cpu_idle_forever(void);
void cpu_enable_interrupts(void);

#endif /* KERNEC_CPU_H */

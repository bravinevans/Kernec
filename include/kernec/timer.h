/* Programmable Interval Timer interface. */
#ifndef KERNEC_TIMER_H
#define KERNEC_TIMER_H

#include <stdint.h>

/* Configure periodic PIT channel 0 and register its IRQ handler. */
int timer_init(uint32_t frequency_hz);
uint64_t timer_ticks(void);

#endif /* KERNEC_TIMER_H */

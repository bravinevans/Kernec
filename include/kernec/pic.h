/* Legacy 8259 PIC interface used during the initial x86_64 bring-up. */
#ifndef KERNEC_PIC_H
#define KERNEC_PIC_H

#include <stdint.h>

void pic_init(void);
int pic_unmask_irq(uint8_t irq);
void pic_send_eoi(uint8_t vector);

#endif /* KERNEC_PIC_H */

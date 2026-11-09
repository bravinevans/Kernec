/* First C entry point for the Kernec kernel. */
#include <kernec/cpu.h>
#include <kernec/gdt.h>
#include <kernec/interrupts.h>
#include <kernec/pic.h>
#include <kernec/serial.h>
#include <kernec/timer.h>

void kmain(void)
{
    serial_init();
    serial_write("\n=====================================\n");
    serial_write("        Kernec Operating System      \n");
    serial_write("=====================================\n");
    serial_write("[KERNEC] Boot entry reached.\n");
    serial_write("Architecture : x86_64\n");
    serial_write("Bootloader   : Limine\n");
    serial_write("Console      : COM1 Serial\n");

    /* Establish selectors before IDT gates use the kernel code selector. */
    gdt_init();
    interrupts_init();
    pic_init();

    if (timer_init(1000u) != 0 || pic_unmask_irq(0u) != 0) {
        serial_write("[KERNEC] Timer/PIC initialization failed; halting.\n");
        cpu_halt_forever();
    }

    serial_write("[KERNEC] GDT and 256-entry IDT installed.\n");
    serial_write("[KERNEC] PIT configured at 1000 Hz; IRQ0 enabled.\n");
    serial_write("[KERNEC] Entering interrupt-driven idle loop.\n");
    cpu_idle_forever();
}

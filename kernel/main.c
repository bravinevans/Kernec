/* First C entry point for the Kernec kernel. */
#include <kernec/cpu.h>
#include <kernec/interrupts.h>
#include <kernec/serial.h>

void kmain(void)
{
    serial_init();
    serial_write("\n=====================================\n");
    serial_write("        Kernec Operating System      \n");
    serial_write("=====================================\n");
    serial_write("[KERNEC] Boot successful.\n");
    serial_write("Architecture : x86_64\n");
    serial_write("Bootloader   : Limine\n");
    serial_write("Console      : COM1 Serial\n");

    interrupts_init();
    serial_write("[KERNEC] 256-entry IDT installed; maskable interrupts remain disabled.\n");

    cpu_halt_forever();
}

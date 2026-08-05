/*
 * File:
 * kernel/main.c
 *
 * Responsibility:
 * First C code executed by the Kernec kernel.
 *
 * Dependencies:
 * kernel/arch/x86_64/serial.c
 *
 * Notes:
 * This file performs only the earliest kernel initialization.
 * Platform-specific work should remain in arch/.
 */

#include <kernec/serial.h>
#include <kernec/cpu.h>

/*
 * Kernel entry point.
 *
 * Control reaches this function from _start in boot.S.
 */
void kmain(void)
{
    serial_init();

    serial_write("\n");
    serial_write("=====================================\n");
    serial_write("        Kernec Operating System      \n");
    serial_write("=====================================\n");
    serial_write("[KERNEC] Boot successful.\n");
    serial_write("Architecture : x86_64\n");
    serial_write("Bootloader   : Limine\n");
    serial_write("Console      : COM1 Serial\n");
    serial_write("\n");

    /*
     * We should never return from kmain().
     */
    cpu_halt_forever();
}

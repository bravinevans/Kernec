# Kernec x86_64 Architecture Bring-up

This directory contains early x86_64 CPU setup and hardware-specific code.

## Current boot-time sequence

1. `boot.S` enters `kmain` with a 64 KiB bootstrap stack.
2. COM1 serial is initialized for diagnostics.
3. `gdt_init()` installs a private kernel GDT (ring-0 code selector `0x08`,
   data selector `0x10`).
4. `interrupts_init()` installs all 256 IDT gates with IF left disabled.
5. `pic_init()` remaps the legacy 8259 PIC to vectors 32-47 and masks all lines.
6. `timer_init(1000)` configures PIT channel 0 and registers the timer handler.
7. Only IRQ0 is unmasked; the kernel enters an interrupt-driven `sti; hlt`
   idle loop. Timer ticks can be read with `timer_ticks()`.

## Interrupt rules and limitations

Handlers execute with maskable interrupts disabled and must be short, bounded,
and non-blocking. They must not sleep or allocate. The dispatcher sends PIC
EOIs after handlers for vectors 32-47. Unhandled CPU exceptions or interrupts
produce serial diagnostics and halt the kernel.

This is a single-processor bring-up implementation, not a production interrupt
subsystem. It does not yet provide TSS/IST stacks (including a dedicated
 double-fault stack), APIC/IOAPIC routing, SMP/per-CPU IDTs, scheduling, deferred
work, or safe user-mode exception recovery. Only the PIT IRQ is unmasked by the
current startup path. Do not enable other IRQs until their handlers and device
acknowledgement paths exist.

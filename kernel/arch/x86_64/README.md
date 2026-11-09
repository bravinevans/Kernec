# Kernec x86_64 Interrupt Subsystem

This architecture-owned module installs a 256-entry x86_64 IDT and supplies a
normalized assembly entry path. Exception vectors are reserved for CPU faults;
all vectors have stubs so an unexpected vector is diagnosed instead of entering
an empty IDT gate.

## Entry and frame

Every stub calls `interrupt_dispatch()` with a pointer to `struct
interrupt_frame`. The assembly path saves general-purpose registers and
normalizes CPU error-code exceptions. The saved frame includes vector, error
code, RIP, CS, and RFLAGS. The optional privilege-transition RSP/SS fields follow
the fixed fields on the CPU stack and are not currently exposed by the C struct.

## Handler registration

`interrupt_register_handler(vector, handler)` registers vectors 32-255. A
registered handler executes in interrupt context, with IF cleared by the IDT
interrupt gate. Handlers must remain short and bounded, must acknowledge their
interrupt controller/device as appropriate, and must not sleep or block. CPU
exceptions and invalid registrations are rejected; an unhandled event halts the
kernel after writing diagnostic state to COM1.

## Safety boundary

`interrupts_init()` executes `cli`, populates and loads the IDT, and deliberately
does not execute `sti`. This milestone does not configure the legacy PIC, APIC,
EOI delivery, per-CPU IDTs, TSS/IST stacks, or nested interrupt policy. Do not
enable maskable interrupts until a controller implementation and its handlers
are initialized. In particular, double-fault IST handling remains future work.

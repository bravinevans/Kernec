# Kernec System Call Subsystem

## Purpose

The System Call subsystem provides the controlled interface between user-space applications and kernel-space services.

It is responsible for:

- Receiving system call requests
- Dispatching requests to the appropriate kernel handler
- Returning results to user space
- Enforcing privilege boundaries

## Directory Layout

```
kernel/syscall/
├── README.md
├── dispatcher.c
├── table.c
├── handlers/
└── include/
```

## Related Specifications

- KERNEC-0012 — System Call Interface

## Related ADRs

- ADR-0001 — Adopt a Monolithic Kernel Architecture
- ADR-0002 — Adopt the x86_64 SYSCALL/SYSRET Mechanism

## Planned Components

- System call entry
- Dispatcher
- System call table
- Handler implementations
- Validation utilities
- Error handling

## Status

Planning

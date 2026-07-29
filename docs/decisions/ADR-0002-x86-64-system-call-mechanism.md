# Architecture Decision Record

**ADR Number:** ADR-0002  
**Title:** Adopt the x86_64 SYSCALL/SYSRET Mechanism  
**Status:** Accepted  
**Date:** 2026-07-29

---

# Context

Kernec requires a secure and efficient mechanism for transitioning execution from user space to kernel space.

On x86_64, several mechanisms exist:

- Software interrupts (`int 0x80`)
- SYSENTER/SYSEXIT
- SYSCALL/SYSRET

The chosen mechanism should align with modern processor capabilities while providing good performance and a clear architectural foundation.

---

# Decision

Kernec will use the **SYSCALL** instruction to enter kernel mode and **SYSRET** to return to user mode on x86_64 systems.

The kernel will configure the required Model Specific Registers (MSRs) during early initialization.

The System Call Dispatcher will receive control immediately after a successful transition into kernel mode.

---

# Alternatives Considered

## Software Interrupt (`int 0x80`)

Advantages

- Simple to understand
- Historically important
- Useful for education

Disadvantages

- Higher overhead
- Legacy interface on x86_64
- Not representative of modern kernels

---

## SYSENTER/SYSEXIT

Advantages

- Faster than software interrupts

Disadvantages

- Primarily designed for 32-bit systems
- Less suitable for a modern x86_64 kernel

---

## SYSCALL/SYSRET

Advantages

- Native x86_64 mechanism
- Lower overhead
- Widely adopted by modern operating systems
- Good long-term foundation

Disadvantages

- Requires correct MSR configuration
- More complex than software interrupts

---

# Consequences

## Positive

- Modern kernel architecture
- Better performance
- Clear separation between user and kernel privilege levels
- Scalable for future syscall expansion

## Negative

- Requires careful CPU initialization
- Architecture-specific implementation

---

# Implementation Notes

The syscall path will eventually include:

Application

↓

SYSCALL

↓

Kernel Entry Stub

↓

System Call Dispatcher

↓

Kernel Service

↓

SYSRET

↓

Application

Architecture-specific assembly should remain isolated under:

kernel/arch/x86_64/

The dispatcher itself should remain architecture-independent whenever practical.

---

# References

- KERNEC-0012 — System Call Interface
- KERNEC-0002 — Target Architecture
- ADR-0001 — Adopt a Monolithic Kernel Architecture

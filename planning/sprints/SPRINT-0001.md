# Sprint 0001

## Name

Boot Infrastructure

---

## Goal

Produce the first bootable Kernec kernel using Limine.

---

## Deliverables

- Linker script
- Makefile
- Limine configuration
- Kernel entry point
- Serial driver
- Bootable ISO
- QEMU launch target

---

## Success Criteria

- Kernel builds without errors.
- ISO is generated.
- QEMU boots successfully.
- "Kernec booted." is printed over COM1.
- CPU halts safely.

---

## Non-goals

- Memory manager
- Scheduler
- Filesystem
- Userspace
- Drivers

---

## Exit Condition

Sprint is complete only when the kernel boots reliably from a clean checkout.

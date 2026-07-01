# KERNEC-0002 — Target Architecture Specification

**Document ID:** KERNEC-0002
**Revision:** 1.0
**Title:** Kernec Operating System — Target Architecture
**Status:** Approved
**Date:** 2026-07-01
**Author:** Lead Implementation Engineer, Kernec Project

---

# 1. Purpose

This document defines the official hardware architecture targets for the Kernec operating system.

It establishes the supported processor architectures, firmware standards, boot process, memory model, and long-term hardware support strategy.

---

# 2. Design Philosophy

Kernec SHALL target modern computing platforms while avoiding unnecessary legacy constraints.

Engineering effort SHALL prioritize maintainability, security, and performance over broad historical hardware compatibility.

---

# 3. Primary Target Architectures

Horizon 1 officially supports:

- x86_64
- AArch64

These architectures SHALL receive full engineering support.

---

# 4. Future Architectures

Future evaluation targets include:

- RISC-V 64
- LoongArch64

Additional architectures MAY be considered after Horizon 2.

---

# 5. Unsupported Architectures

The following SHALL NOT be supported:

- x86 (32-bit)
- IA-64
- ARMv6
- ARMv7
- MIPS
- PowerPC

Support MAY be reconsidered only through a formal Architecture Decision Record.

---

# 6. Firmware

Supported firmware:

- UEFI

Legacy BIOS support is not required.

---

# 7. Bootloader

Initial bootloader:

GRUB 2

Future revisions MAY evaluate:

- Limine
- systemd-boot
- custom Kernec bootloader

---

# 8. Processor Requirements

Supported processors SHALL provide:

- 64-bit execution
- NX/XD support
- SSE4.2 or newer (x86_64)
- Virtual memory support

Recommended features:

- AVX2
- AES-NI
- IOMMU
- SMEP
- SMAP

---

# 9. Memory Model

The operating system SHALL assume:

- virtual memory
- paged memory management
- protected kernel space
- isolated userspace

Future revisions MAY support advanced memory isolation mechanisms.

---

# 10. Endianness

Supported byte order:

- Little Endian

Big-endian architectures are outside Horizon 1.

---

# 11. Filesystems

Initial supported filesystems:

- ext4
- FAT32 (EFI System Partition)

Future support:

- Btrfs
- XFS
- F2FS

---

# 12. Graphics

Initial graphics target:

- Framebuffer
- DRM/KMS

Future:

- Wayland compositor
- hardware acceleration

---

# 13. Storage

Supported storage:

- SATA
- NVMe
- USB Mass Storage

Future support:

- Thunderbolt
- Network boot

---

# 14. Networking

Minimum supported networking:

- Ethernet

Future support:

- Wi-Fi
- Bluetooth

---

# 15. Security Features

Kernec SHALL leverage modern processor security features whenever available, including:

- NX/XD
- SMEP
- SMAP
- ASLR
- Stack canaries

---

# 16. Virtualization

Development SHALL support execution inside:

- QEMU
- KVM

Future testing MAY include:

- VMware
- VirtualBox
- Hyper-V

---

# 17. Long-Term Direction

Future revisions SHALL prioritize:

- hardware abstraction
- architecture independence
- security-first hardware support
- deterministic platform behavior

---

End of KERNEC-0002 Revision 1.0

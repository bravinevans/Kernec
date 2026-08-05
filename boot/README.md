# Kernec Boot Subsystem

The `boot/` directory contains everything required to transition from firmware
execution to the first instruction executed by the Kernec kernel.

This directory owns **boot infrastructure only**.

It does **not** contain kernel runtime code.

Kernel source code belongs in `kernel/`.

Public kernel interfaces belong in `include/`.

Project documentation belongs in `docs/`.

---

# Responsibility

The boot subsystem is responsible for:

- Bootloader configuration
- Kernel linker script
- Boot documentation
- Early boot assets
- ISO boot configuration

It is **not** responsible for:

- Memory management
- Scheduling
- Device drivers
- Filesystems
- Networking
- Userspace

Those responsibilities belong to their respective subsystems.

---

# Engineering Rule

Kernec follows a simple repository principle:

> Every directory must have a single, obvious responsibility.

The `boot/` directory exists so that every file involved in booting can be
found in one place.

---

# Current Boot Architecture

| Component | Decision |
|-----------|----------|
| Bootloader | Limine |
| Firmware | UEFI |
| Kernel Image | ELF64 |
| Target Architecture | x86_64 |
| Build System | GNU Make |
| Emulator | QEMU |

Legacy BIOS support is intentionally outside the scope of Kernec v0.1.

---

# Boot Flow

The intended boot sequence is:

```text
UEFI Firmware
        │
        ▼
Limine Bootloader
        │
        ▼
Reads boot/limine.conf
        │
        ▼
Loads kernec.elf
        │
        ▼
Collects boot information
        │
        ▼
Transfers control to _start
        │
        ▼
Kernel initialization begins
```

---

# Directory Contents

## kernel.ld

Defines how the kernel is linked.

Responsibilities:

- Virtual memory layout
- ELF section layout
- Kernel entry symbol
- Section alignment
- Kernel boundary symbols

---

## limine.conf

Bootloader configuration.

Responsibilities:

- Boot menu entry
- Kernel location
- Boot protocol selection
- Boot options

---

## README.md

Documents the boot subsystem itself.

Every subsystem in Kernec should eventually contain a README explaining its
purpose, boundaries, and responsibilities.

---

# Design Philosophy

Boot should remain as small as possible.

Its only responsibility is delivering execution safely into the kernel.

The kernel should perform kernel work.

The bootloader should perform bootloader work.

Keeping these responsibilities separate produces a simpler and more
maintainable architecture.

---

# Long-Term Roadmap

## v0.1

- Limine
- UEFI
- Kernel ELF
- Serial output
- Initramfs

## v0.2

- Real filesystem
- PCI discovery
- AHCI/NVMe
- switch_root()

## Future

- Secure Boot
- Multiple boot profiles
- Additional architectures

---

# Build Targets

The project Makefile provides:

```text
make
```

Build the kernel.

```text
make iso
```

Generate a bootable ISO image.

```text
make run
```

Launch Kernec in QEMU.

```text
make clean
```

Remove generated build artifacts.

---

# Principle

Boot exists to make the kernel executable.

Nothing more.

Nothing less.

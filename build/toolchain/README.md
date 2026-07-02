# Kernec Toolchain

## Purpose

This directory contains the build toolchain layout used to build the Kernec operating system.

## Required Host Tools

| Tool | Purpose |
|------|---------|
| gcc | C compiler |
| make | Build automation |
| cmake | Build configuration |
| git | Source control |
| nasm | Assembly source compilation |
| qemu-system-x86_64 | Virtual machine testing |
| grub-mkrescue | Bootable ISO creation |
| xorriso | ISO generation |

## Directory Layout

```
toolchain/
├── bin/
├── downloads/
├── patches/
└── sysroot/
```

## Philosophy

The host development environment shall be verified before any build begins.

The build system must fail immediately if required tools are unavailable.

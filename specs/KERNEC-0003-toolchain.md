# KERNEC-0003 — Toolchain Specification

**Document ID:** KERNEC-0003
**Revision:** 1.0
**Title:** Kernec Operating System — Toolchain Specification
**Status:** Approved
**Date:** 2026-07-01
**Author:** Lead Implementation Engineer, Kernec Project

---

# 1. Purpose

This document defines the official development toolchain used to build the Kernec operating system.

Every official build MUST use this specification unless superseded by a future revision.

---

# 2. Engineering Goals

The Kernec toolchain SHALL provide:

- deterministic builds
- reproducibility
- portability
- strong diagnostics
- standards compliance
- long-term maintainability

---

# 3. Host Platforms

Supported build hosts:

- Linux (Primary)
- macOS (Supported)
- FreeBSD (Supported)

Windows is not an official build host.

---

# 4. Target Architectures

Initial supported architectures:

- x86_64
- AArch64

Future architectures MAY include:

- RISC-V 64
- LoongArch
- ARMv7 (evaluation only)

---

# 5. Programming Languages

Core kernel:

- C23

Kernel utilities:

- C23
- Assembly

Userspace:

- C23
- C++23
- Rust (selected components)

Build tooling:

- POSIX Shell
- Python 3

---

# 6. Compiler

Primary compiler:

LLVM Clang

Reasons:

- modern diagnostics
- excellent optimization
- excellent cross-compilation
- LLVM ecosystem

Fallback compiler:

GNU GCC

---

# 7. Linker

Primary:

LLVM LLD

Fallback:

GNU ld

---

# 8. Assembler

Primary:

LLVM Integrated Assembler

Fallback:

GNU as

---

# 9. Build Generator

Official build generator:

CMake

Official backend:

Ninja

---

# 10. Debugging

Primary debugger:

GDB

Secondary debugger:

LLDB

Kernel debugging SHALL support:

- serial debugging
- QEMU debugging
- GDB remote protocol

---

# 11. Binary Utilities

Required utilities:

- llvm-ar
- llvm-objcopy
- llvm-objdump
- llvm-strip
- llvm-readelf

GNU equivalents MAY be used during development.

---

# 12. Version Control

Official VCS:

Git

Primary repository:

GitHub

Main branch:

main

---

# 13. Cross Compilation

Kernec SHALL always support cross-compilation.

The build host SHALL NOT be assumed to be identical to the target architecture.

---

# 14. Build Modes

Supported configurations:

- Debug
- Release
- RelWithDebInfo

---

# 15. Compiler Warnings

Warnings SHALL be treated as errors for official builds.

Required flag:

-Werror

---

# 16. Static Analysis

Preferred tools:

- clang-tidy
- clang-format

Future additions:

- cppcheck
- CodeQL

---

# 17. Formatting

Formatting SHALL be automated.

Manual formatting SHALL NOT be relied upon.

---

# 18. Continuous Integration

Every pull request SHALL:

- compile successfully
- pass formatting checks
- pass static analysis
- pass integration tests

---

# 19. Future Direction

Future revisions MAY migrate completely to LLVM tooling as GNU dependencies are eliminated.

---

End of KERNEC-0003 Revision 1.0

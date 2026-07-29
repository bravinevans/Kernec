# Kernec Commit Convention

**Document ID:** KERNEC-HB-0002  
**Version:** 1.0  
**Status:** Active  
**Project:** Kernec Operating System

---

# 1. Purpose

This document defines the commit message standards used throughout the Kernec project.

A clear Git history helps developers understand:

- What changed
- Why it changed
- Which subsystem was affected

---

# 2. Commit Format

All commits should follow:

type: description


Example:

kernel: initialize interrupt subsystem


---

# 3. Commit Types

## kernel

Changes related to the core kernel.

Examples:

kernel: add kernel entry point
kernel: implement panic handler


---

## mm

Memory management changes.

Examples:

mm: add physical page allocator
mm: implement virtual memory mapping


---

## sched

Scheduler and process management.

Examples:

sched: create task structure
sched: add context switching


---

## fs

Filesystem changes.

Examples:

fs: introduce VFS interface
fs: add inode abstraction


---

## drivers

Hardware driver changes.

Examples:

drivers: add keyboard driver
drivers: implement framebuffer support


---

## arch

Architecture-specific changes.

Examples:

arch: add x86_64 boot sequence
arch: configure CPU initialization


---

## docs

Documentation changes.

Examples:

docs: add architecture specification
docs: update engineering handbook


---

## test

Testing changes.

Examples:

test: add memory allocator tests


---

## build

Build system changes.

Examples:

build: configure kernel compiler flags


---

# 4. Commit Rules

Commits should:

- Describe intent
- Remain focused on one purpose
- Avoid unnecessary changes
- Reference related specifications when possible

---

# 5. Commit Quality

A good commit should answer:

"What changed and why?"

A poor commit:

fixed stuff


A good commit:

mm: implement initial page allocator


---

# 6. Relationship With Specifications

Major features should reference their specification.

Example:

kernel: implement syscall dispatcher (KERNEC-0012)


This creates a traceable relationship:

Specification
↓
Implementation
↓
Commit History


---

# 7. Long-Term Goal

A future developer should be able to understand Kernec's evolution by reading the Git history alone.

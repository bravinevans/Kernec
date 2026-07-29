# KERNEC Engineering Handbook

**Document ID:** KERNEC-HB-0001  
**Version:** 1.0  
**Status:** Active  
**Author:** BRAVIN EVANS KIPLAGAT

---

# 1. Vision

Kernec is an educational and research operating system developed to understand, design, and implement a modern monolithic kernel using disciplined software engineering practices.

The project exists not only to build an operating system, but also to develop deep expertise in systems programming, computer architecture, and kernel engineering.

---

# 2. Engineering Philosophy

Every component should be:

- Understandable
- Testable
- Maintainable
- Documented

No feature should exist without a documented reason.

---

# 3. Core Principles

## Correctness before optimization

Correct software always comes before faster software.

## Simplicity over cleverness

Readable code is preferred over clever code.

## Explicit over implicit

Behavior should be predictable.

## Documentation is part of the implementation

Code is incomplete until it is documented.

## Continuous improvement

Every subsystem should leave the project better than it found it.

---

# 4. Repository Rules

- Keep the repository organized.
- Never commit generated binaries.
- Never commit temporary files.
- Experimental work belongs in dedicated directories.
- Every significant feature begins with a specification.

---

# 5. Engineering Workflow

Every major feature follows this lifecycle:

1. Specification
2. Architecture Review
3. Implementation
4. Testing
5. Documentation
6. Git Commit
7. GitHub Push

---

# 6. Coding Standards

- Prefer descriptive names.
- Keep functions focused on one responsibility.
- Avoid unnecessary global state.
- Comments explain *why*, not *what*.
- Prioritize readability.

---

# 7. Testing Policy

Every subsystem should eventually include automated tests.

Critical kernel functionality should include regression tests.

---

# 8. Documentation Standards

Every subsystem should document:

- Purpose
- Architecture
- Interfaces
- Limitations
- Future Improvements

---

# 9. Commit Message Convention

Write commits that describe intent.

Examples:

kernel: initialize scheduler

mm: implement page allocator

fs: introduce VFS layer

---

# 10. Long-Term Goal

Build a reliable operating system while becoming a disciplined systems engineer through consistent, high-quality engineering.

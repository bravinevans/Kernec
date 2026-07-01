# KERNEC-0007 — Source Tree Specification

**Document ID:** KERNEC-0007
**Revision:** 1.0
**Title:** Kernec Operating System — Source Tree Specification
**Status:** Approved
**Date:** 2026-07-01
**Author:** Lead Implementation Engineer

---

# 1. Purpose

This specification defines the canonical source tree for the Kernec operating system.

No subsystem SHALL introduce new top-level directories without an approved Architecture Decision Record.

---

# 2. Kernel Tree

The kernel source SHALL follow the following layout.

kernel/

    arch/
        x86_64/
        aarch64/

    boot/

    drivers/

    fs/

    include/

    init/

    ipc/

    kernel/

    lib/

    mm/

    sched/

---

# 3. Directory Responsibilities

arch/

Architecture-specific code.

boot/

Early boot initialization.

drivers/

Hardware drivers.

fs/

Filesystem implementation.

include/

Kernel public headers.

init/

Kernel startup.

ipc/

Inter-process communication.

kernel/

Core kernel functionality.

lib/

Kernel utility library.

mm/

Memory management.

sched/

Task scheduler.

---

# 4. Userspace Projects

Each userspace subsystem SHALL remain isolated.

Examples:

kernec-init/

kernec-shell/

kernec-pkg/

kernec-installer/

kernec-ai/

---

# 5. Headers

Public headers:

include/

Private headers:

local subsystem directory.

---

# 6. Tests

Unit tests SHALL reside adjacent to source when practical.

System tests SHALL reside inside:

tests/

---

# 7. Generated Files

Generated files SHALL NEVER be committed.

Generated output SHALL remain inside:

build/

---

# 8. Future Expansion

Future subsystems SHALL require:

- documented ownership
- documented interfaces
- documented dependencies

---

End of KERNEC-0007 Revision 1.0


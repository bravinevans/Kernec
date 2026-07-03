# KERNEC-0009 — Memory Management Architecture

| Field | Value |
| :--- | :--- |
| **Specification ID** | KERNEC-0009 |
| **Title** | Memory Management Architecture |
| **Status** | APPROVED |
| **Revision** | 1.0 |
| **Author** | Kernel Engineering Division, Mammoth Cooperation and Share Holdings |
| **Date** | 2026-07-03 |
| **Dependencies** | KERNEC-0002, KERNEC-0006, KERNEC-0008 |

---

# 1. Purpose

Memory management is one of the most critical subsystems of the Kernec operating system. Every kernel service ultimately depends upon memory being allocated correctly, protected correctly, reclaimed correctly, and isolated correctly.

The objective of this specification is to define the architectural contract governing every physical frame, virtual page, allocator, mapping, ownership rule, protection mechanism, and memory lifecycle inside Kernec.

No implementation may violate this specification.

---

# 2. Engineering Philosophy

> "Before we allocate one byte, we decide how every byte shall behave."

Memory corruption is among the most destructive classes of software failures. Therefore, Kernec treats memory management as an engineering discipline rather than merely an allocation service.

Every allocation must have:

- a defined owner,
- a defined lifetime,
- a defined protection state,
- a defined allocation source,
- a defined deallocation procedure.

No byte shall exist without ownership.

No mapping shall exist without purpose.

---

# 3. Design Goals

The Kernec Memory Manager shall provide:

- deterministic allocation behavior
- strong memory isolation
- minimal fragmentation
- efficient scalability
- security-first design
- maintainable implementation
- comprehensive debugging support
- predictable performance characteristics

Correctness shall always take precedence over raw speed.

---

# 4. Architectural Overview

The memory subsystem is divided into three independent layers.

```
+---------------------------------------+
|        User Applications              |
+---------------------------------------+
                |
                |
          System Calls
                |
+---------------------------------------+
|      Virtual Memory Manager (VMM)     |
+---------------------------------------+
                |
                |
+---------------------------------------+
|    Physical Frame Allocator (PFA)     |
+---------------------------------------+
                |
                |
+---------------------------------------+
|         Physical Hardware RAM         |
+---------------------------------------+
```

Each layer has clearly defined responsibilities.

The Physical Frame Allocator never manages virtual addresses.

The Virtual Memory Manager never allocates physical RAM directly.

Subsystems communicate only through published interfaces.

---

# 5. Physical Memory Model (PMM)

The Physical Memory Manager is responsible for tracking every physical frame installed within the machine.

It operates entirely on physical addresses.

It has no understanding of processes or virtual memory.

---

## 5.1 Physical Frame Size

Kernec adopts a fixed physical frame size.

| Property | Value |
|-----------|-------|
| Frame Size | 4096 Bytes |
| Alignment | 4096 Bytes |
| Page Size | 4096 Bytes |

Every allocation shall be aligned to the native frame size.

---

## 5.2 Terminology

For consistency throughout all engineering documentation:

| Term | Definition |
|-------|------------|
| Frame | One physical 4 KiB memory block |
| Page | One virtual 4 KiB memory block |
| Mapping | Association between page and frame |
| Region | Consecutive collection of pages |
| Heap | Dynamically allocated memory |
| Pool | Reserved allocator region |

---

## 5.3 Physical Memory States

Every physical frame shall exist in exactly one state.

| State | Description |
|--------|-------------|
| FREE | Available for allocation |
| RESERVED | Reserved by firmware or hardware |
| KERNEL | Used by the kernel |
| USER | Owned by user processes |
| DEVICE | Reserved for MMIO or DMA |
| SHARED | Shared between processes |
| CORRUPTED | Removed from allocator due to detected corruption |

A frame shall never simultaneously exist in multiple states.

---

## 5.4 Physical Frame Ownership

Every frame shall have an owner.

Possible ownership classes include:

- OWNER_KERNEL
- OWNER_PROCESS(pid)
- OWNER_DEVICE(device_id)
- OWNER_FREE

Ownership metadata shall be maintained independently from allocation metadata.

---

# 6. Physical Frame Allocation

The allocator shall operate in multiple phases.

---

## 6.1 Bootstrap Allocator

During early boot, before the virtual memory subsystem becomes operational, Kernec shall utilize a Bitmap Allocator.

Characteristics:

- simple implementation
- deterministic behavior
- minimal initialization requirements
- zero dynamic memory allocation

The bootstrap allocator exists only during system initialization.

After kernel initialization completes, it shall be permanently retired.

---

## 6.2 Primary Allocator

The production allocator shall utilize the Buddy Allocation algorithm.

Reasons include:

- efficient contiguous allocations
- automatic block merging
- logarithmic allocation complexity
- reduced fragmentation

The buddy allocator shall become the permanent physical memory allocator after bootstrap completion.

---

## 6.3 Reserved Physical Regions

The allocator shall permanently reserve:

- Kernel image
- Bootloader memory
- Initial page tables
- ACPI structures
- APIC memory
- PCI MMIO regions
- Firmware reserved memory
- Memory map structures

Reserved regions shall never be returned by allocation routines.

---

# 7. Virtual Memory Model

Kernec utilizes a Higher-Half Kernel architecture.

User space and kernel space remain permanently isolated.

Kernel mappings exist inside every process.

User mappings remain process-specific.

This design provides:

- simplified kernel access
- process isolation
- consistent kernel addresses
- improved debugging

---

# 8. Canonical Address Layout

The x86_64 address space shall be divided into three logical regions.

| Virtual Range | Purpose |
|---------------|----------|
| 0x0000000000000000 – 0x00007FFFFFFFFFFF | User Space |
| 0x0000800000000000 – 0xFFFF7FFFFFFFFFFF | Non-Canonical Guard Region |
| 0xFFFF800000000000 – 0xFFFFFFFFFFFFFFFF | Kernel Space |

The non-canonical region acts as a hardware-enforced guard area.

No mappings shall exist within this range.

---

# 9. Kernel Virtual Address Layout

The kernel shall divide its virtual address space into dedicated regions.

| Region | Purpose |
|----------|---------|
| Direct Physical Map | Direct RAM mapping |
| Kernel Heap | Dynamic kernel allocations |
| VMALLOC | Large virtual allocations |
| Kernel Image | Kernel code and read-only data |
| Per-CPU Area | CPU-local storage |
| Device MMIO | Hardware mappings |
| Reserved | Future expansion |

Each region shall maintain strict protection boundaries.

Subsystems shall not allocate memory outside their assigned regions.

---

# 10. User Address Space

Each process shall receive an independent virtual address space.

Typical layout:

```
High Address
----------------------------
User Stack
Guard Page

Shared Libraries

Memory Mapped Files

Heap

Program Image

NULL Guard Page
----------------------------
Low Address
```

User address layouts shall support Address Space Layout Randomization (ASLR).

Guard pages shall be inserted wherever practical to detect overflows.

---

# 11. Page Tables

Kernec utilizes standard x86_64 four-level paging.

Hierarchy:

- PML4
- PDPT
- Page Directory
- Page Table

Standard page size:

4096 bytes

Supported huge pages:

- 2 MiB
- 1 GiB

Huge pages shall be limited to:

- Direct Physical Map
- Kernel Image

They shall not be used for general heap allocations.

---

# 12. Memory Protection

Memory protection forms the primary security boundary between kernel space, user space, and hardware.

Every mapping shall explicitly define its protection attributes.

Implicit permissions are forbidden.

---

## 12.1 Protection Flags

Every page mapping shall define one or more of the following flags.

| Flag | Description |
|------|-------------|
| READ | Page may be read |
| WRITE | Page may be modified |
| EXECUTE | Page may contain executable instructions |
| USER | Accessible from Ring 3 |
| GLOBAL | Shared across all address spaces |
| NO_CACHE | Disable CPU caching |
| WRITE_THROUGH | Enable write-through caching |

---

## 12.2 W^X Policy

Kernec adopts a strict **Write XOR Execute (W^X)** policy.

No memory page shall ever be writable and executable simultaneously.

Examples:

| Region | Permissions |
|---------|-------------|
| Kernel Code | Read + Execute |
| Kernel Data | Read + Write |
| User Code | Read + Execute + User |
| User Heap | Read + Write + User |
| Device MMIO | Read + Write + No Cache |

Violations shall be rejected by the Virtual Memory Manager.

---

## 12.3 Guard Pages

Guard pages shall remain permanently unmapped.

They are used to detect:

- stack overflows
- stack underflows
- heap overruns
- invalid pointer arithmetic

Accessing a guard page shall immediately generate a page fault.

---

# 13. Kernel Heap

Dynamic kernel memory shall be provided through the Kernel Heap subsystem.

Kernel memory differs fundamentally from user memory.

Kernel allocations must prioritize:

- determinism
- stability
- fragmentation resistance
- debugging capability

---

## 13.1 Initial Allocator

The first implementation shall utilize a Free List Allocator.

Reasons:

- simple implementation
- deterministic behavior
- easy debugging
- minimal code complexity

Supported allocation classes:

16 B

32 B

64 B

128 B

256 B

512 B

1024 B

2048 B

Larger allocations shall be serviced directly by the Physical Frame Allocator.

---

## 13.2 Future Allocator

After system stabilization, the Free List allocator shall be replaced with a Slab Allocator.

Benefits include:

- object caching
- reduced fragmentation
- constant-time allocation
- lower cache misses

Typical cached objects include:

- task structures
- process descriptors
- page descriptors
- vnode structures
- IPC endpoints

---

# 14. Allocation Interfaces

Kernel subsystems shall allocate memory only through approved interfaces.

Direct manipulation of page tables or physical memory is prohibited.

---

## 14.1 Physical Frame Allocator

```c
phys_addr_t pfa_alloc_frames(size_t count);

void pfa_free_frames(
    phys_addr_t address,
    size_t count
);
```

---

## 14.2 Kernel Heap

```c
void *kmalloc(
    size_t size,
    uint32_t flags
);

void kfree(
    void *ptr
);
```

Supported allocation flags:

```c
KMEM_NORMAL
KMEM_ATOMIC
KMEM_ZERO
```

---

## 14.3 Virtual Memory

```c
int vmm_map_pages(
    page_table_t *table,
    virt_addr_t virtual_address,
    phys_addr_t physical_address,
    size_t pages,
    uint32_t protection
);
```

```c
int vmm_unmap_pages(
    page_table_t *table,
    virt_addr_t virtual_address,
    size_t pages
);
```

```c
int vmm_protect_pages(
    page_table_t *table,
    virt_addr_t virtual_address,
    size_t pages,
    uint32_t protection
);
```

These interfaces form the official Memory Management API.

No subsystem may bypass them.

---

# 15. Page Fault Handling

Page faults are expected events and shall not automatically trigger kernel panic.

The page fault handler shall determine the cause before deciding the response.

Possible causes include:

- unmapped page
- protection violation
- write to read-only page
- execute on non-executable page
- kernel NULL dereference
- user NULL dereference

Responses include:

| Cause | Action |
|--------|--------|
| User access violation | Terminate offending process |
| Kernel programming error | Panic |
| Demand paging (future) | Allocate page |
| Copy-on-write (future) | Duplicate frame |

---

# 16. Memory Ownership

Every allocated page shall possess exactly one owner.

Possible owners include:

- Kernel
- User Process
- Driver
- DMA Device
- Shared Object

Ownership metadata shall accompany allocation metadata throughout the page lifecycle.

Ownership transfers shall be explicit.

Implicit ownership changes are prohibited.

---

# 17. Reference Counting

Shared physical frames shall maintain atomic reference counts.

Examples include:

- shared libraries
- shared memory
- memory-mapped files
- IPC buffers

A frame shall only return to the Physical Frame Allocator when its reference count reaches zero.

---

# 18. DMA Memory

Memory used for Direct Memory Access requires special handling.

DMA buffers shall:

- remain physically contiguous when required
- remain page aligned
- remain cache coherent
- never migrate while active

The DMA subsystem shall coordinate with the Physical Frame Allocator.

---

# 19. TLB Management

Whenever page mappings change, stale Translation Lookaside Buffer (TLB) entries shall be invalidated.

Methods include:

- single-page invalidation
- address-space invalidation
- full TLB flush

Future multiprocessor systems shall perform coordinated TLB shootdowns across CPU cores.

---

# 20. Memory Lifecycle

Every allocation shall follow the same lifecycle.

```
Allocate
    │
    ▼
Map
    │
    ▼
Use
    │
    ▼
Unmap
    │
    ▼
Free
```

Skipping lifecycle stages is forbidden.

Every subsystem shall leave memory in a valid state regardless of success or failure.

---

# 21. Debugging & Diagnostics

Memory defects are among the most difficult classes of kernel bugs to diagnose.

Kernec shall therefore incorporate memory debugging facilities directly into the Memory Management subsystem.

These facilities may be enabled through compile-time configuration options.

---

## 21.1 Memory Poisoning

When debugging is enabled, memory shall be filled with diagnostic values.

| State | Pattern |
|--------|----------|
| Newly Allocated | 0xCD |
| Freed Memory | 0xFD |
| Guard Regions | 0xCC |

These patterns assist developers in identifying:

- use-after-free
- uninitialized memory
- buffer overruns
- stale pointers

---

## 21.2 Redzones

Kernel heap allocations shall include optional guard regions.

Before releasing memory, the allocator shall verify that guard regions remain intact.

Corrupted guard regions indicate buffer overflow.

Kernel response:

- emit diagnostic information
- identify offending allocation
- panic in debug builds

---

## 21.3 Allocation Tracking

Every allocation may optionally record:

- source file
- function
- line number
- allocation size
- allocation timestamp
- owning subsystem

Upon kernel shutdown or panic, the allocator may generate a leak report listing all outstanding allocations.

---

# 22. Out-of-Memory Policy

Kernec does not depend upon swap space for kernel survival.

Kernel memory exhaustion is considered a critical event.

---

## 22.1 Kernel Allocations

Normal allocations may attempt memory reclamation.

If reclamation fails:

- emit diagnostics
- preserve logs
- invoke kernel panic

Continuing execution without critical memory is considered unsafe.

---

## 22.2 Atomic Allocations

Atomic allocations occur in contexts where sleeping is prohibited.

If insufficient memory exists:

- return NULL
- never block
- never invoke the scheduler

The caller is responsible for graceful recovery.

---

## 22.3 User Space

User allocations shall return:

```
-ENOMEM
```

where possible.

Future implementations may invoke an Out-of-Memory Manager to reclaim resources from selected user processes.

---

# 23. Memory Invariants

The following rules shall always remain true.

1. Every allocated page has exactly one owner.
2. Every mapping has explicit permissions.
3. No writable executable pages exist.
4. Kernel memory is inaccessible from user mode.
5. Physical frames are never leaked.
6. Every allocation is eventually freed.
7. Every page table remains internally consistent.
8. Guard pages remain unmapped.
9. Reference counts never become negative.
10. Memory corruption is never ignored.

Violation of these invariants constitutes a kernel defect.

---

# 24. Future Development

The following capabilities are reserved for future implementation.

## Planned Features

- Copy-on-Write (CoW)
- Demand Paging
- Memory Compression
- NUMA Awareness
- Huge Page Optimization
- Memory Hot-Plug
- Transparent Huge Pages
- Shared Memory Optimizations
- Slab Allocator
- Kernel Address Space Layout Randomization (KASLR)
- Page Cache
- Swap Support
- Memory Encryption
- Secure Memory Regions

Each feature shall receive its own engineering specification before implementation.

---

# 25. Implementation Roadmap

Development shall proceed incrementally.

## Phase 1

Bootstrap Memory Manager

- Parse bootloader memory map
- Bitmap allocator
- Early page tables

---

## Phase 2

Physical Memory Manager

- Buddy allocator
- Frame ownership
- Reference counting

---

## Phase 3

Virtual Memory Manager

- Virtual mappings
- Page protection
- Higher-half kernel
- Kernel heap

---

## Phase 4

Memory Hardening

- W^X enforcement
- Debug poisoning
- Redzones
- Leak detection

---

## Phase 5

Advanced Features

- Copy-on-Write
- Demand paging
- Slab allocator
- Huge pages
- NUMA

---

# 26. Glossary

| Term | Definition |
|------|------------|
| Page | Virtual memory unit |
| Frame | Physical memory unit |
| PMM | Physical Memory Manager |
| VMM | Virtual Memory Manager |
| PFA | Physical Frame Allocator |
| TLB | Translation Lookaside Buffer |
| MMU | Memory Management Unit |
| DMA | Direct Memory Access |
| W^X | Write XOR Execute |
| CoW | Copy-on-Write |
| NUMA | Non-Uniform Memory Access |

---

# 27. Engineering Principles

The Memory Management subsystem shall always prioritize:

1. Correctness before optimization.
2. Predictability before cleverness.
3. Security before convenience.
4. Maintainability before complexity.
5. Explicit ownership before implicit behavior.

Every change shall preserve these principles.

---

# 28. Approval

This specification defines the authoritative architecture of memory management within the Kernec kernel.

No implementation shall contradict this document without formal revision through the Kernec Engineering Specification process.

---

**End of Specification**

**KERNEC-0009 — Memory Management Architecture**

**Revision:** 1.0

**Status:** APPROVED

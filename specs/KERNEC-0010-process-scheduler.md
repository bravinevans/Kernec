# KERNEC-0010 — Process & Scheduler Architecture

| Field | Value |
| :--- | :--- |
| **Specification ID** | KERNEC-0010 |
| **Title** | Process & Scheduler Architecture |
| **Status** | **APPROVED** |
| **Revision** | 1.0 |
| **Author** | Kernel Engineering Division, Mammoth Cooperation and Share Holdings |
| **Date** | 2026-07-04 |
| **Dependencies** | KERNEC-0008 (Kernel Architecture), KERNEC-0009 (Memory Management) |

---

# 1. Introduction

> *"Execution is the heartbeat of the operating system. Memory defines what exists; the scheduler decides what lives in time."*

The Process and Scheduler subsystem is responsible for determining which execution context is permitted to use the processor at any given moment.

While the Memory Manager defines ownership of memory resources, the Scheduler defines ownership of processor time.

Every scheduling decision directly affects responsiveness, throughput, fairness, latency, scalability, and overall user experience.

This specification establishes the architectural contract governing process management, thread management, context switching, scheduling policies, processor utilization, and synchronization requirements throughout the Kernec kernel.

No implementation may violate the rules defined herein.

---

# 2. Design Philosophy

The scheduler shall be engineered according to the following principles.

## 2.1 Reliability Before Speed

Scheduling correctness always takes precedence over optimization.

A scheduler that produces occasional race conditions or deadlocks is considered defective regardless of benchmark performance.

---

## 2.2 Fairness

Runnable tasks should receive processor time appropriate to their scheduling class and priority.

No runnable task should experience permanent starvation.

---

## 2.3 Predictability

Scheduling behavior must remain deterministic whenever practical.

Kernel developers must always be able to reason about why a particular thread was selected.

---

## 2.4 Scalability

The scheduler shall scale efficiently from a single processor to many logical CPU cores without requiring architectural redesign.

---

## 2.5 Separation of Policy and Mechanism

Scheduling algorithms are policies.

Context switching, interrupt handling, and runqueue management are mechanisms.

Mechanisms should remain reusable even if scheduling policies evolve in future releases.

---

# 3. Scheduler Goals

The scheduler shall satisfy the following engineering objectives.

• Low latency for interactive workloads.

• Fair CPU distribution among runnable tasks.

• Efficient processor utilization.

• Deterministic execution for real-time workloads.

• Minimal context-switch overhead.

• Scalability across multiple processor cores.

• Starvation prevention.

• Stable behavior under heavy system load.

---

# 4. Process and Thread Model

Kernec distinguishes clearly between a process and a thread.

A process owns resources.

A thread performs execution.

This distinction is mandatory throughout the kernel.

---

## 4.1 Process

A process is the resource container for one or more execution threads.

A process owns:

- Virtual address space
- File descriptors
- Credentials
- Signal handlers
- IPC resources
- Memory mappings
- Security tokens
- Environment information

A process does not execute instructions directly.

---

## 4.2 Thread

A thread represents a single execution context.

Each thread owns:

- CPU register state
- Stack pointers
- Scheduling metadata
- Execution priority
- Processor affinity
- Time slice accounting
- Floating-point context

Threads are the entities scheduled by the kernel.

---

## 4.3 Threading Model

Kernec implements a strict **1:1 threading model.**

One user thread corresponds to one kernel thread.

Advantages include:

- Simpler scheduling
- Better SMP scalability
- Native multicore execution
- Easier debugging
- Predictable synchronization

---

# 5. Core Kernel Structures

The following structures define the architectural contract.

```c
struct kernec_process {
    pid_t pid;
    pid_t ppid;

    page_table_t *page_table;

    struct files_struct *files;

    struct credentials *cred;

    struct list_head threads;
};

struct kernec_thread {

    tid_t tid;

    struct kernec_process *process;

    struct cpu_context context;

    void *kernel_stack;

    void *user_stack;

    uint32_t state;

    uint32_t priority;

    uint64_t runtime;

    uint64_t time_slice;

    cpu_id_t cpu;

    struct list_head run_node;

    struct list_head process_node;
};
```

The exact implementation may evolve, but these architectural responsibilities shall remain unchanged.

---

# 6. Thread Lifecycle

Every thread exists in exactly one execution state.

Transitions between states shall follow strict rules.

## Thread States

| State | Description |
|--------|-------------|
| THREAD_NEW | Newly created but not yet runnable |
| THREAD_READY | Waiting in a runqueue |
| THREAD_RUNNING | Currently executing |
| THREAD_SLEEPING | Waiting for an event |
| THREAD_BLOCKED | Waiting on synchronization primitives |
| THREAD_ZOMBIE | Execution complete, awaiting cleanup |
| THREAD_DEAD | Destroyed and resources released |

---

## State Transition Rules

The following transitions are permitted.

```
THREAD_NEW
      │
      ▼
THREAD_READY
      │
      ▼
THREAD_RUNNING
     /   \
    /     \
   ▼       ▼
SLEEP   BLOCKED
   │       │
   └──► READY
             │
             ▼
         RUNNING
             │
             ▼
        THREAD_ZOMBIE
             │
             ▼
         THREAD_DEAD
```

The following transitions are explicitly forbidden.

- SLEEPING → RUNNING
- BLOCKED → RUNNING
- DEAD → READY
- DEAD → RUNNING
- ZOMBIE → RUNNING

All sleeping or blocked threads must first transition to READY before receiving CPU time.

---

# 7. Process Lifecycle

A process progresses through the following stages.

1. Creation
2. Resource initialization
3. Thread creation
4. Scheduler admission
5. Execution
6. Waiting
7. Termination
8. Zombie state
9. Resource cleanup
10. PID recycling

No process shall skip lifecycle stages.

---

# 8. Process Ownership Rules

Every thread belongs to exactly one process.

A process may own multiple threads.

Every thread shall inherit:

- Address space
- Credentials
- File descriptor table
- Signal handlers
- Security context

from its parent process.

The scheduler schedules threads.

It never schedules processes directly.

---

# 9. Fundamental Scheduler Rules

The scheduler shall obey the following invariants.

1. A running thread exists on exactly one CPU.

2. A READY thread exists in exactly one runqueue.

3. A sleeping thread exists in no runqueue.

4. A blocked thread exists in no runqueue.

5. A thread cannot simultaneously be READY and RUNNING.

6. Scheduler decisions shall always preserve system consistency.

7. Scheduler metadata shall never be modified without appropriate synchronization.

---

**End of Part 1**

# 10. Scheduling Architecture

Kernec employs a fully preemptive scheduler.

The scheduler is responsible for determining which READY thread shall execute whenever:

- A timer interrupt occurs.
- A running thread voluntarily yields.
- A higher-priority thread becomes runnable.
- A running thread blocks.
- A processor becomes idle.

Scheduling decisions shall always be deterministic and repeatable.

---

## 10.1 Minimum Viable Scheduler

The first production scheduler shall implement a **Per-CPU Multi-Level Feedback Queue (MLFQ)**.

Each logical processor maintains its own independent runqueue.

This minimizes lock contention while providing excellent scalability on multicore systems.

---

## 10.2 Queue Levels

Each processor maintains four scheduling queues.

| Queue | Priority | Intended Workload |
|--------|----------|------------------|
| Queue 0 | Highest | Interactive tasks |
| Queue 1 | High | General applications |
| Queue 2 | Medium | Background workloads |
| Queue 3 | Lowest | CPU-intensive processing |

Higher queues always execute before lower queues.

---

## 10.3 Time Slice Allocation

Each scheduling level receives a predefined execution quantum.

| Queue | Time Slice |
|--------|-----------:|
| Queue 0 | 5 ms |
| Queue 1 | 10 ms |
| Queue 2 | 20 ms |
| Queue 3 | 40 ms |

Long-running CPU-bound tasks gradually migrate toward lower queues.

Interactive workloads naturally remain near the highest queues.

---

## 10.4 Starvation Prevention

No runnable thread shall starve indefinitely.

To guarantee fairness, all runnable threads shall receive a periodic priority boost.

The default boost interval shall be approximately one second.

During a priority boost:

- Every runnable thread is promoted to Queue 0.
- Time slice accounting is reset.
- Interactive responsiveness is restored.

---

# 11. Scheduling Classes

Kernec defines several scheduling classes.

| Class | Description |
|--------|-------------|
| SCHED_IDLE | Executes only when no other runnable work exists |
| SCHED_NORMAL | Standard time-sharing workloads |
| SCHED_BATCH | Long-running background computation |
| SCHED_RR | Real-Time Round Robin |
| SCHED_FIFO | Real-Time First-In First-Out |

Real-time scheduling classes are reserved for privileged software.

---

## 11.1 Scheduling Priorities

Within each scheduling class, threads possess numerical priorities.

Higher numerical priorities execute before lower priorities.

Priority inheritance mechanisms shall be introduced in future specifications to prevent priority inversion.

---

# 12. Context Switching

Context switching transfers processor execution from one thread to another.

This operation is one of the most performance-critical routines inside Kernec.

Every context switch shall preserve:

- General-purpose registers
- Stack pointer
- Instruction pointer
- Flags register
- Floating-point state
- SIMD registers
- Processor control registers when required

---

## 12.1 Context Switch Flow

A normal context switch proceeds through the following sequence.

```
Running Thread
      │
      ▼
Save CPU Registers
      │
      ▼
Save Stack Pointer
      │
      ▼
Choose Next Thread
      │
      ▼
Load New Stack
      │
      ▼
Restore Registers
      │
      ▼
Resume Execution
```

Interrupts shall remain disabled during the critical context-switch window.

---

## 12.2 Lazy Floating Point Switching

Floating-point and SIMD state shall utilize lazy switching.

Instead of saving floating-point registers during every context switch, the kernel shall defer restoration until a thread actually performs floating-point operations.

This reduces unnecessary overhead.

---

# 13. Kernel Threads

Kernel threads execute entirely in Ring 0.

Unlike user threads, kernel threads do not possess independent user address spaces.

Kernel threads inherit the global kernel page tables.

They are primarily responsible for:

- Memory reclamation
- Deferred work
- Filesystem maintenance
- Device management
- Background housekeeping
- Future networking tasks

Kernel threads shall never execute user code.

---

# 14. Idle Threads

Every logical processor shall own exactly one idle thread.

The idle thread is permanently resident.

When no runnable work exists, the scheduler dispatches the idle thread.

The idle thread repeatedly executes the processor's halt instruction.

```
while (true)
{
    halt_cpu();
}
```

The idle thread shall:

- Never terminate.
- Never sleep.
- Never block.
- Never acquire long-duration locks.

---

# 15. Scheduler Synchronization

Scheduler correctness depends upon strict synchronization.

Runqueue modifications shall always occur while holding the corresponding runqueue lock.

The scheduler shall support:

- Spinlocks
- Mutexes
- Read-write locks
- Wait queues
- Atomic operations

Blocking synchronization primitives shall never be used within interrupt context.

Spinlocks shall remain the primary synchronization primitive for scheduler internals.

---

# 16. Preemption

Kernec utilizes kernel preemption.

Preemption allows higher-priority work to interrupt currently executing threads.

Preemption shall occur when:

- Timer interrupts expire.
- Higher-priority threads awaken.
- Explicit yield operations occur.
- Processor load balancing requires migration.

Critical sections may temporarily disable preemption.

Preemption shall always be restored before returning to user execution.

---

**End of Part 2**

---

# 8. Symmetric Multiprocessing (SMP)

Kernec is designed for modern multi-core processors from the beginning. The scheduler shall scale across CPUs without relying on a single global run queue.

## 8.1 Per-CPU Run Queues

Each logical processor maintains its own scheduler state.

Every CPU owns:

- Local run queue
- Idle thread
- Scheduler statistics
- Scheduler lock
- Current running thread

Advantages:

- Reduced lock contention
- Better CPU cache locality
- Excellent scalability

---

## 8.2 Load Balancing

The scheduler continuously balances work between CPUs.

Two balancing mechanisms exist:

### Push Balancing

When creating a new thread:

- Determine least loaded CPU
- Insert thread into that CPU's queue

### Pull Balancing

When a CPU becomes idle:

- Search neighboring CPUs
- Steal runnable threads
- Resume execution

---

## 8.3 CPU Affinity

Threads may optionally define CPU affinity.

Affinity modes include:

- Any CPU
- Preferred CPU
- CPU mask
- Exclusive CPU (future)

The scheduler attempts to respect affinity unless doing so would severely reduce performance.

---

## 9. Scheduling Invariants

The scheduler obeys several mathematical rules that must never be violated.

### Run Queue Lock Rule

A run queue may only be modified while holding its scheduler lock.

---

### Single Ownership Rule

A thread exists in exactly one place:

- Running
- Ready Queue
- Sleeping
- Zombie

Never multiple simultaneously.

---

### Interrupt Rule

Scheduling decisions occur with interrupts disabled.

---

### Preemption Rule

Preemption only occurs when:

```
preempt_count == 0
```

Critical sections increase the preemption counter.

---

### Idle Rule

Only the idle thread may execute:

```
HLT
```

No ordinary thread may halt the CPU.

---

## 10. Public Scheduler Interfaces

The scheduler exposes stable internal APIs.

```c
void schedule(void);

void wake_up_process(struct kernec_thread *thread);

void sleep_on(wait_queue_t *queue);

struct kernec_thread *
kthread_create(
    void (*func)(void *),
    void *data,
    const char *name);

void preempt_check(void);
```

Subsystems must never manipulate run queues directly.

---

## 11. Debugging Support

Debug builds include scheduler diagnostics.

Features include:

- Context switch tracing
- Run queue visualization
- Deadlock detection
- Priority inversion detection
- Scheduler latency statistics
- Per-thread execution accounting

These facilities are disabled in production builds.

---

## 12. Future Expansion

Reserved roadmap items include:

- Completely Fair Scheduler (CFS)
- Energy-aware scheduling
- NUMA-aware balancing
- Deadline scheduling
- CPU isolation
- Real-time improvements
- Tickless scheduling
- Scheduler classes for virtualization

---

## 13. Implementation Roadmap

### Phase 1

Bootstrap scheduler.

- Thread structures
- Process structures
- Idle thread
- Init thread

---

### Phase 2

Context switching.

- Assembly switch routine
- Register save/restore
- Kernel stacks

---

### Phase 3

Run queues.

- Ready queues
- Priority queues
- Timer tick
- Time slicing

---

### Phase 4

Blocking.

- Sleep queues
- Wake-up logic
- Synchronization integration

---

### Phase 5

SMP.

- Per-CPU scheduler
- Load balancing
- CPU affinity
- Inter-Processor Interrupts (IPI)

---

### Phase 6

User processes.

- Process creation
- fork()
- exec()
- Thread creation
- Full multitasking

---

# End of Specification

**KERNEC-0010 — Process & Scheduler Architecture**

This specification defines the execution model of Kernec. It establishes the contracts governing processes, threads, scheduling, context switching, and multi-core execution. All future scheduler implementations shall conform to this specification unless superseded by a formally approved engineering revision.

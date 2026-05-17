# POSIX Concurrent Systems Lab

> Two C projects demonstrating **OS-level concurrency primitives** on Linux/POSIX: a multi-process supermarket simulation built around **shared memory** and **named semaphores**, plus a multithreaded intruder-detection prototype using **pthreads**.

---

## Tech Stack

![C](https://img.shields.io/badge/C-A8B9CC?style=for-the-badge&logo=c&logoColor=white)
![POSIX](https://img.shields.io/badge/POSIX-3776AB?style=for-the-badge)
![pthreads](https://img.shields.io/badge/pthreads-FF6600?style=for-the-badge)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![Make](https://img.shields.io/badge/GNU_Make-A42E2B?style=for-the-badge)

---

## Why this repo exists

University coursework was where I first wrote code that reasoned at the OS layer: forking child processes, allocating shared-memory segments with `shmget`, gating critical sections with named semaphores, and joining pthreads. Both projects in this repository exercise primitives that high-level languages usually hide. Recruiters scanning for backend, distributed-systems, or systems-engineering experience can read these two projects end-to-end in about 15 minutes and see exactly how I reason about concurrency.

---

## Repository Layout

```
.
├── README.md
├── .gitignore
├── supermarket_simulation/                    # ENCS3340 Project 2 (final submission, scrubbed)
│   ├── main.c                                 # Spawns the 5 child processes and the IPC primitives
│   ├── customer.c                             # Customer arrival / shopping / checkout process
│   ├── drawer.c                               # Checkout drawer + payment handling process
│   ├── shelving_team.c                        # Restocking team process
│   ├── timer.c                                # Global clock / shift-end signaller process
│   ├── customer_header.h, shelving_team.h
│   ├── functions.h, header.h
│   ├── constants.h, processing_times.h
│   ├── semphores.h                            # Wrappers around sem_open / sem_wait / sem_post
│   ├── shared_memories.h                      # Wrappers around shmget / shmat / shmdt
│   ├── makefile, run.sh, config.txt
│   ├── supermarket_items.txt, names.txt       # Simulation inputs
└── intruder_detection_skeleton/               # Multithreaded prototype (draft state)
    └── intruder_detection_pthreads_skeleton.c # Ultrasonic + camera + biometric gate, pthreads
```

---

## Project A — Supermarket Real-Time Simulation

A discrete-event simulation of a supermarket day, modelled as five cooperating Linux processes.

### Architecture at a glance

```
                        +---------+
                        |  main   |  spawns and joins everyone
                        +----+----+
                             |
       +---------+-----------+---------+-------------+
       |         |                     |             |
   customer   shelving_team          timer        drawer
   (N procs)  (M procs)              (1 proc)     (K procs)

Shared memory (shm) carries:                Named semaphores arbitrate:
  - shelf inventory                           - shelf-restock lock
  - checkout queue                            - drawer-checkout queue
  - simulation clock                          - shift-end broadcast
```

### IPC primitives used

| Primitive | Where it lives | What it protects |
| --- | --- | --- |
| `shmget` / `shmat` / `shmdt` | `shared_memories.h` | Shelf inventory and checkout queue shared between every process |
| `sem_open` / `sem_wait` / `sem_post` | `semphores.h` | Mutual exclusion on shelf access and on the cashier drawer |
| `fork` + `exec` | `main.c` | Per-process customer / shelver / drawer instantiation |
| `signal` / `kill` | `timer.c` | End-of-shift broadcast to all children |

### Build and run

```bash
cd supermarket_simulation
make
./run.sh        # or ./main directly, after tuning config.txt
```

The build is the standard `make` flow targeting GCC on Linux. After `make`, five executables are produced (excluded from version control via `.gitignore`).

---

## Project B — Intruder-Detection pthreads Skeleton

A draft prototype demonstrating multi-threaded sensor fusion for a security gate. Three pthreads run concurrently:

| Thread | Role |
| --- | --- |
| `ultrasonic_monitor` | Polls 6 ultrasonic sensors, averages distance, opens gate 1 when an object is closer than the threshold |
| `camera_monitor` | Triggers 3 cameras, fuses 4 detection algorithms per camera, computes total intrusion probability |
| `intruder_response` | When triggered, activates lights + sirens, disables gate 1, performs biometric (hand + eye) verification, conditionally re-enables the gate |

> **Status: skeleton / draft.** The file documents an architecture but contains placeholder syscalls and one or two unresolved syntax issues from the original assignment draft. It is published as a study of *how* the threads would be wired together, not as a buildable program. Hardware glue functions (`capture_ultrasonic`, `activate_light`, etc.) are stubbed.

---

## Co-Author Consent

The supermarket simulation (Project A) was co-authored at Birzeit University with **Jana Herzallah** and **Lana Badwan** (the original submission folder was named `supermarketJAL` — J=Jana, A=Ahmed, L=Lana). It is published here with the explicit consent of both co-authors. The intruder-detection skeleton (Project B) is solo draft work.

---

## What I would do differently today

This is academic code from 2024 and it shows. If I rebuilt the supermarket simulation now I would: replace the bare `sem_open` wrappers with a small RAII-style helper layer, swap signal-based shift-end broadcast for a `pthread_cond_t` on shared memory, and add a `valgrind --tool=helgrind` pass to the makefile. The intruder-detection draft would become a state machine rather than three free-running threads racing on shared flags. I am keeping the original code untouched so the repo accurately reflects the coursework as submitted.

# POSIX Concurrent Systems Lab

![C](https://img.shields.io/badge/C-A8B9CC?style=for-the-badge&logo=c&logoColor=white)
![POSIX](https://img.shields.io/badge/POSIX-3776AB?style=for-the-badge)
![pthreads](https://img.shields.io/badge/pthreads-FF6600?style=for-the-badge)
![Shared_Memory](https://img.shields.io/badge/Shared_Memory-654FF0?style=for-the-badge)
![Named_Semaphores](https://img.shields.io/badge/Named_Semaphores-EE0000?style=for-the-badge)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![GNU_Make](https://img.shields.io/badge/GNU_Make-A42E2B?style=for-the-badge)

> Two **OS-level C** projects exercising the full POSIX concurrency stack — a five-process supermarket simulation built on **shared memory** and **named semaphores**, and a multithreaded intruder-detection prototype using **pthreads** — written for the Linux/POSIX environment that backend, infrastructure, and systems-engineering teams care about.

---

## Architecture & Key Features

### Project A — Supermarket Real-Time Simulation (multi-process, IPC-heavy)

A discrete-event simulation of a supermarket day modelled as five cooperating Linux processes spawned and joined from `main.c`.

| Concern | Primitive Used | Where it lives |
| --- | --- | --- |
| Inventory and queue state shared across all 5 processes | `shmget` / `shmat` / `shmdt` **(shared memory)** | `shared_memories.h` |
| Mutual exclusion on shelf access and on the cashier drawer | `sem_open` / `sem_wait` / `sem_post` **(named semaphores)** | `semphores.h` |
| Per-process instantiation (N customers + M shelvers + K drawers + 1 timer) | `fork` + `exec` | `main.c` |
| End-of-shift broadcast to every child process | POSIX `signal` / `kill` | `timer.c` |

```
                          +---------+
                          |  main   |   spawns and joins everyone
                          +----+----+
                               |
         +---------+-----------+---------+-------------+
         |         |                     |             |
     customer   shelving_team          timer        drawer
     (N procs)  (M procs)              (1 proc)     (K procs)

  Shared memory (shm) carries:                 Named semaphores arbitrate:
    - shelf inventory                            - shelf-restock lock
    - checkout queue                             - drawer-checkout queue
    - simulation clock                           - shift-end broadcast
```

### Project B — Intruder-Detection pthreads Skeleton (multi-threaded prototype)

Three concurrent **pthreads** running inside a single process, demonstrating sensor-fusion architecture for a smart security gate:

| Thread | Responsibility |
| --- | --- |
| `ultrasonic_monitor` | Polls 6 ultrasonic sensors, averages distances, opens gate 1 below a proximity threshold. |
| `camera_monitor` | Triggers 3 cameras, fuses 4 detection algorithms per camera, computes total intrusion probability. |
| `intruder_response` | Activates lights + sirens, disables gate 1, runs biometric (hand + eye) verification, conditionally re-enables the gate. |

> **Status:** documentation-grade skeleton. The file demonstrates *how* the threads are wired but contains stub syscalls and a small number of unresolved syntax issues from the original assignment draft — published as an architecture reference, not as a buildable program.

### Repository Layout

```
.
├── README.md
├── .gitignore
├── supermarket_simulation/                    # 18 source files, build with `make`
│   ├── main.c, customer.c, drawer.c, shelving_team.c, timer.c
│   ├── header.h, functions.h, constants.h, processing_times.h
│   ├── semphores.h, shared_memories.h
│   ├── customer_header.h, shelving_team.h
│   ├── makefile, run.sh, config.txt
│   └── supermarket_items.txt, names.txt
└── intruder_detection_skeleton/
    └── intruder_detection_pthreads_skeleton.c
```

### Build & Run (Supermarket)

```bash
cd supermarket_simulation
make
./run.sh        # or ./main directly, after tuning config.txt
```

### Hygiene Posture

Before commit: stripped the embedded `.git/` (teammate identities), `.idea/` (JetBrains state), and 5 pre-compiled Linux ELF binaries (`main`, `drawer`, `customer`, `shelving_team`, `timer`). Those binary names are also covered by `.gitignore` to prevent regression.

---

## Co-Author Consent

The supermarket simulation (Project A) was co-authored at Birzeit University with **Jana Herzallah** (1201139) and **Lana Badwan** (1200071) — the original submission folder was named `supermarketJAL` (J = Jana, A = Ahmed, L = Lana). It is published here with the explicit consent of both co-authors. The intruder-detection skeleton (Project B) is solo draft work.

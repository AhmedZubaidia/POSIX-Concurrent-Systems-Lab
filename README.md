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

### Project A — Supermarket Real-Time Simulation (multi-process, IPC-heavy, OpenGL-rendered)

A discrete-event simulation of a supermarket day modelled as five cooperating Linux processes spawned and joined from `main.c`, with a live **OpenGL / GLUT** renderer (`drawer`) that paints the shared state every frame.

| Concern | Primitive Used | Where it lives |
| --- | --- | --- |
| Inventory, customer roster, and clock state shared across all 5 processes | `shmget` / `shmat` / `shmdt` **(shared memory)** | `opengl-visualization/include/shared_memories.h` |
| Mutual exclusion on shelves, storage bins, and the customer queue | POSIX semaphores (`sys/sem.h`) | `opengl-visualization/include/semphores.h` |
| Per-process instantiation (orchestrator + customer + shelving_team + drawer + timer) | `fork` + `execlp` | `opengl-visualization/src/main.c` |
| Worker pool inside `shelving_team` (manager threads dispatch worker threads) | `pthread.h` | `opengl-visualization/src/shelving_team.c` |
| Live visualization of shelves, storage, teams, customers, and wall clock | OpenGL / GLUT (`freeglut3-dev`) | `opengl-visualization/src/drawer.c` |
| End-of-shift broadcast and child cleanup | POSIX `signal` + `waitpid` | `opengl-visualization/src/timer.c` + `main.c` |

```
                          +---------+
                          |  main   |   reads config, owns IPC, forks & reaps
                          +----+----+
                               |
         +---------+-----------+---------+-------------+
         |         |                     |             |
     customer   shelving_team          timer        drawer
     (N procs)  (M procs, threaded)    (1 proc)     (1 proc, OpenGL renderer)

  Shared memory carries:                       POSIX semaphores arbitrate:
    - shelf inventory                            - per-shelf restock lock
    - storage inventory                          - per-bin storage lock
    - customer roster                            - customer queue
    - simulated wall clock                       - shutdown broadcast
```

A rendered frame is captured at [`opengl-visualization/docs/showcase.png`](opengl-visualization/docs/showcase.png).

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
├── opengl-visualization/                      # Project A — concurrency backend + OpenGL frontend
│   ├── src/
│   │   ├── main.c           # Orchestrator: reads config.txt, owns IPC, forks workers
│   │   ├── customer.c       # Customer agents (per-process, threaded)
│   │   ├── shelving_team.c  # Manager + worker thread pool that refills shelves
│   │   ├── drawer.c         # OpenGL / GLUT renderer (live visualization)
│   │   └── timer.c          # Simulated wall clock + open/close window
│   ├── include/
│   │   ├── constants.h, header.h, functions.h, processing_times.h
│   │   ├── shared_memories.h, semphores.h
│   │   └── customer_header.h, shelving_team.h
│   ├── docs/showcase.png    # Rendered simulation frame
│   ├── makefile, run.sh
│   ├── config.txt, names.txt, supermarket_items.txt
│   └── README.md            # Detailed build, configuration, and architecture reference
└── intruder_detection_skeleton/
    └── intruder_detection_pthreads_skeleton.c
```

### Build & Run (Supermarket with live OpenGL visualization)

System dependencies (Debian / Ubuntu):

```bash
sudo apt update
sudo apt install build-essential freeglut3-dev libglu1-mesa-dev libgl1-mesa-dev
```

Then:

```bash
cd opengl-visualization
make
./run.sh        # or: ./main, after tuning config.txt
```

### Hygiene Posture

Before publishing the upstream Project-A sources, stripped the embedded `.git/` (teammate identities), `.idea/` (JetBrains state), and the pre-compiled Linux ELF binaries (`main`, `drawer`, `customer`, `shelving_team`, `timer`). The `.gitignore` in `opengl-visualization/` covers those binary names to prevent regression.

---

## Repository Consolidation Note

This repository now cleanly encapsulates **both** halves of the Real-Time Systems coursework in one place:

1. **Concurrency backend** — the POSIX multi-process / multi-threaded simulation (`opengl-visualization/src/main.c`, `customer.c`, `shelving_team.c`, `timer.c`) coordinating five cooperating Linux processes through System V shared memory, POSIX semaphores, and `pthreads`.
2. **OpenGL visualization frontend** — the dedicated renderer process (`opengl-visualization/src/drawer.c`) that attaches to the same shared memory each frame and paints storage, shelves, both shelving teams, the customer grid, and a synchronized wall clock using GLUT.

Previously the backend lived in a flat `supermarket_simulation/` folder here while the OpenGL-rendered build was published as a separate standalone repository. Both have been merged into the single `opengl-visualization/` subdirectory above so that the backend code and its graphic frontend now live, build, and ship together as one coherent project — no cross-repository hopping required to reproduce the demo.

---

## Co-Author Consent

The supermarket simulation (Project A) was co-authored at Birzeit University with **Jana Herzallah** (1201139) and **Lana Badwan** (1200071) — the original submission folder was named `supermarketJAL` (J = Jana, A = Ahmed, L = Lana). It is published here with the explicit consent of both co-authors. The intruder-detection skeleton (Project B) is solo draft work.

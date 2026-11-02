# Cigarette Smokers Problem — POSIX Synchronization

A multithreaded implementation of the classic Cigarette Smokers Problem using C, POSIX Pthreads, semaphores, and mutex locks.

## Overview

The simulation consists of one agent thread and three smoker threads. Each smoker owns one ingredient: tobacco, paper, or matches.

The agent supplies two ingredients per round. Only the smoker who owns the missing ingredient is allowed to proceed.

## Technologies

- C
- POSIX Pthreads (`pthread.h`)
- POSIX Semaphores (`semaphore.h`)
- Mutex Locks
- GCC
- Ubuntu Linux

## Features

- Agent and smoker thread coordination
- Semaphore-based synchronization
- Mutex-protected shared state
- Clean thread termination
- Round-robin scheduling
- Multithreaded logging and testing

## Project Files

- `smokers.c` — Main simulation and thread synchronization
- `testing.c` — Logging and counter management
- `testing.h` — Testing function declarations
- `test_main.c` — Standalone multithreaded logging test

## Build and Run

### Main Simulation

```bash
gcc -Wall -Wextra -pthread smokers.c testing.c -o smokers
./smokers
```

Expected results:

```text
Smoker 1 smoked 4 times.
Smoker 2 smoked 4 times.
Smoker 3 smoked 4 times.
```

### Logging Test

```bash
gcc -Wall -Wextra -pthread test_main.c testing.c -o test_logging
./test_logging
```

Expected results: 100 recording operations per smoker.

## Testing

The integrated simulation successfully completed 10 consecutive execution tests.

The standalone logging module was also tested using three concurrent threads performing 100 recording operations each.

## Academic Context

Developed collaboratively by Leen Al Ahmad and Laila for Operating Systems Analysis & Design (SYST30102) at Sheridan College.

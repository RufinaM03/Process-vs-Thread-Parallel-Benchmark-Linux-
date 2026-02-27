# Process vs Thread Parallel Benchmark (Linux)

This project compares multi-process and multi-threaded parallel computation models in Linux using C.

## Features

- Multi-process execution using `fork()` and `pipe()`
- Multi-threaded execution using `pthread`
- High-resolution timing using `clock_gettime`
- Workload scaling via command-line arguments
- Performance comparison between process and thread models

## Compilation

```bash
gcc process_thread_benchmark.c -o benchmark -lpthread
```
## Concepts Demonstrated

This project explores core Linux systems programming concepts:

- Process lifecycle management (`fork`, `wait`)
- Inter-process communication via pipes
- Multithreading using POSIX threads (`pthread`)
- Synchronization and execution coordination
- Parallel workload execution and aggregation
- High-resolution performance benchmarking (`clock_gettime`)

---

## Performance Analysis

- Thread-based execution generally exhibits lower overhead due to shared memory space.
- Process-based execution introduces additional cost from process creation and IPC.
- Performance scaling is influenced by CPU core availability and workload granularity.
- Overhead becomes more visible with smaller workloads.

## Sample Output
<img width="559" height="336" alt="image" src="https://github.com/user-attachments/assets/7d8e62f8-75cb-40ae-926a-edf93cb0f5cf" />

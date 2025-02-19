Parallel Firewall

A high-performance parallel firewall implementation utilizing POSIX threads (pthreads) and the Producer-Consumer algorithm with a circular buffer. Designed for efficient packet filtering in multi-threaded environments.

Features

Multi-threaded Processing – Uses pthreads for concurrent packet handling.

Producer-Consumer Model – Implements a circular buffer for efficient task distribution.

Configurable Filtering Rules – Supports rule-based packet filtering.

Scalability – Adapts to different workloads by adjusting thread count.

Performance-Oriented – Optimized for speed and resource efficiency.

Synchronization Mechanisms – Uses mutex locks and condition variables to ensure safe multi-threaded access.

Thread-Safe Implementation – Prevents race conditions through proper synchronization.

Organised output – Packages are finished in the order that they were processed, FIFO.

Architecture

Packet Producers – Receive incoming packets and enqueue them into the circular buffer.

Circular Buffer – Acts as a shared queue between producers and consumers, implemented with mutex locks and condition variables.

Packet Consumers – Retrieve packets from the buffer, apply firewall rules, and determine whether to allow or drop them.

Synchronization Mechanisms – Uses pthread_mutex_t for mutual exclusion and pthread_cond_t for thread signaling to prevent race conditions and ensure efficient task coordination.

Build & Usage

Requirements

GCC or Clang (C compiler supporting pthreads)

Make (optional, for build automation)

Linux/macOS (for pthread support)

Compilation

To build the project:

make

Or manually:

gcc -o parallel_firewall src/*.c -lpthread

Running the Firewall

./parallel_firewall [num_producers] [num_consumers] [buffer_size]

Example:

./parallel_firewall 4 4 128

This starts the firewall with 4 producer threads, 4 consumer threads, and a circular buffer of size 128.

Why Use This Parallel Firewall?

Efficient packet processing with multi-threading.

Real-time adaptability by tuning producer-consumer ratio.

Thread-safe design using mutex and condition variables.

Improves throughput in high-load network scenarios.

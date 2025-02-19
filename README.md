# Parallel Firewall

A high-performance parallel firewall implementation utilizing **POSIX threads (pthreads)** and the **Producer-Consumer algorithm** with a circular buffer. Designed for efficient packet filtering in multi-threaded environments.

## Features

- **Multi-threaded Processing** – Uses `pthreads` for concurrent packet handling.
- **Producer-Consumer Model** – Implements a circular buffer for efficient task distribution.
- **Configurable Filtering Rules** – Supports rule-based packet filtering.
- **Scalability** – Adapts to different workloads by adjusting thread count.
- **Performance-Oriented** – Optimized for speed and resource efficiency.
- **Synchronization Mechanisms** – Uses `mutex` locks and `condition variables` to ensure safe multi-threaded access.
- **Thread-Safe Implementation** – Prevents race conditions through proper synchronization.
- **Organized Output** – Ensures packets are processed in FIFO (First In, First Out) order.

## Architecture

1. **Packet Producers** – Receive incoming packets and enqueue them into the circular buffer.
2. **Circular Buffer** – Acts as a shared queue between producers and consumers, implemented with `mutex` locks and `condition variables`.
3. **Packet Consumers** – Retrieve packets from the buffer, apply firewall rules, and determine whether to allow or drop them.
4. **Synchronization Mechanisms** – Uses `pthread_mutex_t` for mutual exclusion and `pthread_cond_t` for thread signaling to prevent race conditions and ensure efficient task coordination.

## Build & Usage

### Requirements
- **GCC** or **Clang** (C compiler supporting pthreads)
- **Make** (optional, for build automation)
- **Linux/macOS** (for pthread support)

### Compilation
To build the project:
```sh
make
```

This starts the firewall with **4 producer threads**, **4 consumer threads**, and a **circular buffer of size 128**.

## Why Use This Parallel Firewall?

- 🚀 **Efficient packet processing** with multi-threading.
- ⚡ **Real-time adaptability** by tuning producer-consumer ratio.
- 🔒 **Thread-safe design** using `mutex` and `condition variables`.
- 📈 **Improves throughput** in high-load network scenarios.

## Future Enhancements

- Implement dynamic load balancing.
- Support for additional filtering criteria.
- Performance benchmarking and optimization.

## License

This project is licensed under the **MIT License** – feel free to use and modify it!

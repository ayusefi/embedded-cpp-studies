# Producer-Consumer Project

## Overview
This project demonstrates the classic Producer-Consumer problem in C++ and explores common concurrency issues such as race conditions and busy-waiting. It shows the evolution of solutions from a buggy implementation to a robust, efficient version using mutexes and condition variables.

## Problem Statement
The Producer-Consumer problem involves two types of threads:
- **Producer:** Generates data and adds it to a shared buffer.
- **Consumer:** Removes data from the buffer for processing.

The challenge is to synchronize access to the shared buffer to prevent data corruption and ensure efficient operation.

## Key Concepts
### Race Conditions
A race condition occurs when multiple threads access shared data concurrently, and the outcome depends on the timing of their execution. This can lead to unpredictable behavior and bugs.

### Busy-Waiting
Busy-waiting is a synchronization technique where a thread repeatedly checks a condition in a loop, consuming CPU cycles unnecessarily. This is inefficient and should be avoided in favor of proper synchronization mechanisms.

## Code Evolution

### 1. Buggy Version (Race Condition)
In the initial implementation, both producer and consumer threads access the shared buffer without any synchronization. This leads to race conditions, where data may be lost or corrupted.

**Key Issues:**
- No protection for shared data.
- Unpredictable results due to concurrent access.

```cpp
// Example: Buggy producer-consumer (no synchronization)
std::vector<int> buffer;

void producer() {
    for (int i = 0; i < 100; ++i) {
        buffer.push_back(i); // No protection
    }
}

void consumer() {
    while (!buffer.empty()) {
        int item = buffer.back();
        buffer.pop_back(); // No protection
    }
}
```

### 2. Mutex-Only Version (Busy-Waiting)
To fix race conditions, a mutex is introduced to protect the buffer. However, the consumer uses busy-waiting to check for available data, leading to inefficient CPU usage.

**Key Issues:**
- Race conditions are fixed.
- Busy-waiting wastes CPU cycles.

```cpp
std::vector<int> buffer;
std::mutex mtx;

void producer() {
    for (int i = 0; i < 100; ++i) {
        std::lock_guard<std::mutex> lock(mtx);
        buffer.push_back(i);
    }
}

void consumer() {
    while (true) {
        std::lock_guard<std::mutex> lock(mtx);
        if (!buffer.empty()) {
            int item = buffer.back();
            buffer.pop_back();
        } else {
            // Busy-wait: keep looping
        }
    }
}
```

### 3. Efficient Version (Condition Variable)
The final version uses a condition variable to efficiently synchronize producer and consumer threads. The consumer waits for data to become available without busy-waiting, and the producer notifies the consumer when new data is added.

**Key Features:**
- No race conditions.
- No busy-waiting; threads sleep until needed.
- Efficient CPU usage.

```cpp
std::vector<int> buffer;
std::mutex mtx;
std::condition_variable cv;
bool done = false;

void producer() {
    for (int i = 0; i < 100; ++i) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            buffer.push_back(i);
        }
        cv.notify_one();
    }
    {
        std::lock_guard<std::mutex> lock(mtx);
        done = true;
    }
    cv.notify_one();
}

void consumer() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return !buffer.empty() || done; });
        if (!buffer.empty()) {
            int item = buffer.back();
            buffer.pop_back();
        } else if (done) {
            break;
        }
    }
}
```

## How to Run
1. Build the project using your preferred C++ compiler (e.g., `g++`).
2. Run the executable to observe the producer-consumer behavior.

## Conclusion
This project illustrates the importance of proper synchronization in concurrent programming. By evolving from a buggy implementation to an efficient solution, it highlights best practices for avoiding race conditions and busy-waiting in C++.

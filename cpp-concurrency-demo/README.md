# C++ Concurrency Demo: Race Condition with std::queue

This demo shows a classic race condition using C++11 threads and a shared `std::queue<int>`.

## Buggy Code Example

```cpp
#include <iostream>
#include <thread>
#include <queue>

std::queue<int> sharedQueue;

void producer() {
    for (int i = 0; i < 10000; ++i) {
        sharedQueue.push(i);
        std::cout << "Produced: " << i << std::endl;
    }
}

void consumer() {
    for (int i = 0; i < 10000; ++i) {
        if (!sharedQueue.empty()) {
            int value = sharedQueue.front();
            sharedQueue.pop();
            std::cout << "Consumed: " << value << std::endl;
        }
    }
}

int main() {
    std::thread t1(producer);
    std::thread t2(consumer);
    t1.join();
    t2.join();
    return 0;
}
```

## Why This Fails
- `std::queue` is **not thread-safe**. Both threads access and modify the queue at the same time.
- This can cause crashes, corrupted data, or undefined behavior (e.g., segmentation faults, missing or duplicated values).
- The race condition occurs because there is no synchronization protecting the shared resource.

Run this code to observe the problem, then learn how to fix it using mutexes in the next step.

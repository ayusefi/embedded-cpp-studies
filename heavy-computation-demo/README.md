# Heavy Computation Demo

This project demonstrates the impact of synchronous heavy computation on a responsive main loop in C++.

## Theory: std::async and std::future

`std::async` is a high-level C++ tool for running functions asynchronously. When you call `std::async`, it may run the function in a new thread or defer execution until you need the result. It returns a `std::future` object, which will eventually hold the result of the computation. You can use `future.get()` to retrieve the result, blocking until it's ready.

**Example:**
```cpp
#include <future>
#include <iostream>

int compute() {
    // Simulate heavy work
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return 42;
}

int main() {
    std::future<int> result = std::async(std::launch::async, compute);
    // Do other work here...
    std::cout << "Waiting for result..." << std::endl;
    std::cout << "Result: " << result.get() << std::endl;
    return 0;
}
```

## Synchronous Heavy Computation Demo

This demo shows how calling a heavy function synchronously blocks the main loop and makes the program unresponsive.

- The main loop prints a dot every 100ms.
- The heavy function simulates a long computation (2-3 seconds).
- When called synchronously, the main loop freezes until the computation is done.

See `main.cpp` for the code.

## How to Run

1. Open a terminal in this folder.
2. Compile the program:
   
   ```bash
   g++ -std=c++11 -pthread main.cpp -o heavy_demo
   ```

3. Run the executable:
   
   ```bash
   ./heavy_demo
   ```

You will see the main loop printing dots. When the heavy computation runs synchronously, the dots will pause until the computation finishes, then resume.

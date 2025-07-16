# Embedded C++ Studies

## Overview
This repository contains comprehensive C++ concurrency and embedded systems programming studies, with a focus on thread-safe programming, synchronization primitives, and real-time system design. These projects provide hands-on experience with standard C++ concurrency tools that are essential for embedded systems and ROS 2 development.

## Learning Goals
- Master standard C++ concurrency tools (`std::thread`, `std::mutex`, `std::condition_variable`)
- Understand thread-safe data exchange patterns
- Apply geometric control theory concepts in multi-threaded environments
- Prepare for ROS 2 development with proper synchronization patterns
- Build expertise in embedded systems programming

## Projects

### 1. Producer-Consumer Demo (`producer-consumer-demo/`)
**Focus**: Classic Producer-Consumer problem implementation

**Key Concepts**:
- Thread synchronization with mutexes and condition variables
- Race condition demonstration and prevention
- Efficient thread coordination vs. busy-waiting
- Thread-safe queue implementation

**Files**:
- `producer_consumer.cpp` - Complete demonstration of three implementation approaches
- `simple_demo.cpp` - Clean, efficient implementation example
- `BUILD_AND_RUN.md` - Comprehensive build and usage instructions

**Quick Start**:
```bash
cd producer-consumer-demo
make run-simple  # For quick demonstration
make run         # For complete educational demo
```

### 2. C++ Concurrency Demo (`cpp-concurrency-demo/`)
**Focus**: Race condition examples and thread safety

**Key Concepts**:
- Race condition visualization
- Thread-safe queue implementation
- Mutex usage patterns

**Quick Start**:
```bash
cd cpp-concurrency-demo
./race_condition  # If already built
```

### 3. Heavy Computation Demo (`heavy-computation-demo/`)
**Focus**: Multi-threaded computational workloads

**Key Concepts**:
- Parallel processing patterns
- Thread pool concepts
- Performance optimization

**Quick Start**:
```bash
cd heavy-computation-demo
./heavy_demo  # If already built
```

### 4. Smart Pointers (`SmartPointers/`)
**Focus**: Modern C++ memory management

**Key Concepts**:
- RAII principles
- `std::unique_ptr` and `std::shared_ptr`
- Memory safety in multi-threaded environments

**Quick Start**:
```bash
cd SmartPointers
# See BUILD_AND_RUN.md for instructions
```

## Quick Setup

### Prerequisites
- C++11 compatible compiler (g++ 4.9+ or clang++ 3.5+)
- POSIX threads support
- Make utility

### Build All Projects
```bash
# Build individual projects
cd producer-consumer-demo && make
cd ../cpp-concurrency-demo && make  # If Makefile exists
cd ../heavy-computation-demo && make  # If Makefile exists
cd ../SmartPointers && make  # If Makefile exists
```

## Core C++ Concurrency Concepts Covered

### Threading Primitives
- `std::thread` - Basic parallel execution
- `std::mutex` - Mutual exclusion
- `std::condition_variable` - Efficient thread coordination
- `std::atomic` - Lock-free atomic operations

### Synchronization Patterns
- Producer-Consumer
- Reader-Writer
- Thread-safe data structures
- RAII with lock guards

### Best Practices
- Avoiding race conditions
- Preventing deadlocks
- Efficient resource sharing
- Exception safety in concurrent code

## ROS 2 Integration Concepts
These studies prepare you for ROS 2 development:
- **Node synchronization**: Similar to producer-consumer patterns
- **Message passing**: Thread-safe queue implementations
- **Service coordination**: Request-response synchronization
- **Real-time constraints**: Efficient synchronization for deterministic behavior

## Advanced Topics Covered
- Lock-free programming concepts
- Memory ordering and consistency
- Performance analysis of synchronization primitives
- Real-time scheduling considerations
- Geometric control theory applications in concurrent systems

## Troubleshooting Common Issues

### Build Issues
```bash
# Missing pthread library
g++ -pthread your_file.cpp

# C++11 features not available
g++ -std=c++11 your_file.cpp
```

### Runtime Issues
- **Deadlocks**: Check lock ordering and condition variable usage
- **Race conditions**: Ensure proper mutex protection
- **High CPU usage**: Replace busy-waiting with condition variables

## Further Reading
- C++ Concurrency in Action by Anthony Williams
- The Art of Multiprocessor Programming by Maurice Herlihy
- ROS 2 Design Documentation on thread safety
- Modern C++ concurrency patterns and best practices

## Contributing
When adding new projects:
1. Create a dedicated directory
2. Include comprehensive README.md
3. Add BUILD_AND_RUN.md with clear instructions
4. Update this main README with project information
5. Ensure code follows modern C++ best practices


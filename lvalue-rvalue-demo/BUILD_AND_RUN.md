# Build and Run Instructions

This directory contains several C++ programs demonstrating L-values, R-values, and move semantics.

## Prerequisites
- C++17 or later compiler (g++, clang++)
- Make (optional, for convenience)

## Individual Programs

### 1. Basic Examples
```bash
g++ -std=c++17 -Wall -Wextra -O2 basic_examples.cpp -o basic_examples
./basic_examples
```

### 2. Overload Resolution
```bash
g++ -std=c++17 -Wall -Wextra -O2 overload_resolution.cpp -o overload_resolution
./overload_resolution
```

### 3. Move Semantics
```bash
g++ -std=c++17 -Wall -Wextra -O2 move_semantics.cpp -o move_semantics
./move_semantics
```

### 4. Reference Types
```bash
g++ -std=c++17 -Wall -Wextra -O2 reference_types.cpp -o reference_types
./reference_types
```

### 5. Performance Benchmark
```bash
g++ -std=c++17 -Wall -Wextra -O2 performance_benchmark.cpp -o performance_benchmark
./performance_benchmark
```

## Run All Programs
```bash
make all
make run
```

## Clean Build Artifacts
```bash
make clean
```

## Program Descriptions

- **basic_examples**: Fundamental concepts of L-values and R-values
- **overload_resolution**: Function overloading with different reference types
- **move_semantics**: Resource management and move operations
- **reference_types**: Comprehensive reference type analysis
- **performance_benchmark**: Quantitative performance comparison of copy vs move

## Learning Path

1. Start with `basic_examples` to understand core concepts
2. Run `overload_resolution` to see how the compiler chooses functions
3. Study `move_semantics` for practical resource management
4. Finish with `reference_types` for advanced reference behavior
5. Run `performance_benchmark` to see quantitative performance differences

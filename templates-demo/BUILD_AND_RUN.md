# Build and Run Instructions

This directory contains comprehensive C++ template demonstrations covering both function templates and class templates.

## Prerequisites
- C++17 or later compiler (g++, clang++)
- Make (optional, for convenience)

## Individual Programs

### 1. Function Templates
```bash
g++ -std=c++17 -Wall -Wextra -O2 function_templates.cpp -o function_templates
./function_templates
```

### 2. Class Templates
```bash
g++ -std=c++17 -Wall -Wextra -O2 class_templates.cpp -o class_templates
./class_templates
```

### 3. Advanced Templates
```bash
g++ -std=c++17 -Wall -Wextra -O2 advanced_templates.cpp -o advanced_templates
./advanced_templates
```

### 4. Embedded Systems Templates
```bash
g++ -std=c++17 -Wall -Wextra -O2 embedded_templates.cpp -o embedded_templates
./embedded_templates
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

- **function_templates**: Basic function template concepts and syntax
- **class_templates**: Generic classes including Pair, Array, Stack, and Calculator
- **advanced_templates**: Variadic templates, SFINAE, and metaprogramming
- **embedded_templates**: Templates for embedded systems (GPIO, memory pools, ring buffers)

## Learning Path

1. Start with `function_templates` to understand basic template syntax
2. Move to `class_templates` to see how the STL works internally
3. Study `advanced_templates` for modern C++ template features
4. Explore `embedded_templates` for practical embedded applications

## Key Concepts Demonstrated

### Function Templates
- Generic functions that work with multiple types
- Template parameter deduction
- Function template specialization
- Multiple template parameters

### Class Templates
- Generic classes (like STL containers)
- Template instantiation
- Non-type template parameters
- Class template specialization

### Advanced Features
- Variadic templates (C++11)
- SFINAE (Substitution Failure Is Not An Error)
- Template metaprogramming
- Compile-time computation

### Embedded Applications
- Fixed-point arithmetic templates
- Type-safe GPIO pin management
- Memory pool templates for deterministic allocation
- Ring buffer templates for embedded systems

## Template Benefits

1. **Zero Runtime Overhead**: All resolution at compile time
2. **Type Safety**: Compile-time type checking prevents runtime errors
3. **Code Reuse**: Write once, use with multiple types
4. **Performance**: No virtual function calls or dynamic dispatch
5. **STL Foundation**: Understanding how std::vector, std::map, etc. work

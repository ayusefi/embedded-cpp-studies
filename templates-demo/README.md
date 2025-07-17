# C++ Templates Demonstration

## Overview

This project demonstrates C++ templates - both function templates and class templates. Templates are fundamental to modern C++ and are the foundation of the Standard Template Library (STL).

## Core Concepts

### Function Templates
- **Generic functions** that work with multiple data types
- Syntax: `template <typename T>`
- Compiler generates specific versions for each type used
- Enable code reuse without sacrificing type safety

### Class Templates
- **Generic classes** that can work with different types
- Foundation of STL containers like `std::vector<T>`, `std::map<K,V>`
- Allow creation of type-safe, reusable data structures

## Files in This Demo

### Function Templates
- **`function_templates.cpp`**: Basic function template examples
- **`advanced_function_templates.cpp`**: Multiple template parameters, specialization

### Class Templates
- **`class_templates.cpp`**: Basic class template examples
- **`advanced_class_templates.cpp`**: Template specialization, non-type parameters

### Practical Examples
- **`template_containers.cpp`**: Custom container implementations
- **`embedded_templates.cpp`**: Templates for embedded systems

## Key Benefits for Embedded Systems

1. **Zero Runtime Overhead**: Templates are resolved at compile time
2. **Type Safety**: Compile-time type checking prevents runtime errors
3. **Code Reuse**: Write once, use with multiple types
4. **Performance**: No virtual function calls or dynamic dispatch
5. **Memory Efficiency**: No additional memory overhead

## Learning Path

1. Start with `function_templates.cpp` for basic concepts
2. Explore `class_templates.cpp` for generic classes
3. Study advanced examples for real-world applications
4. See embedded-specific use cases in `embedded_templates.cpp`

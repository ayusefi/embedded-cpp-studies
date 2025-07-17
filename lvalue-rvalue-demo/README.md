# L-values and R-values in C++

## Theory Overview

### L-values (Left-values)
- **Definition**: Expressions that refer to a memory location and can appear on the left side of an assignment
- **Characteristics**:
  - Have a persistent memory address
  - Can be assigned to (unless const)
  - Have a name or can be dereferenced
  - Examples: variables, array elements, dereferenced pointers

### R-values (Right-values)
- **Definition**: Temporary expressions that don't have a persistent location
- **Characteristics**:
  - Temporary values that will be destroyed after the expression
  - Cannot appear on the left side of an assignment
  - Don't have a persistent memory address
  - Examples: literals, temporary objects, results of expressions

### R-value References (&&)
- **Added in C++11** to enable move semantics and perfect forwarding
- Allow binding to temporary objects
- Enable efficient transfer of resources without copying
- Key to implementing move constructors and move assignment operators

## Examples in this Demo

1. **basic_examples.cpp**: Simple demonstrations of L-values vs R-values
2. **overload_resolution.cpp**: Function overloading with L-value and R-value references
3. **move_semantics.cpp**: Practical move semantics implementation
4. **reference_types.cpp**: Comprehensive reference type examples
5. **performance_benchmark.cpp**: Quantitative performance comparison of copy vs move

## Key Concepts Demonstrated

- L-value and R-value identification
- Function overload resolution based on value categories
- Move semantics and resource transfer
- Perfect forwarding
- Reference collapsing rules
- Quantitative performance analysis of copy vs move operations

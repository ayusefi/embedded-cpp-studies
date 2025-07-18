# C++ STL Algorithms Mastery Guide

## Overview
This project demonstrates the power of C++ Standard Library algorithms combined with lambda expressions, comparing traditional loop-based approaches with modern STL algorithm approaches.

## Key STL Algorithms Covered

### 1. `std::for_each`
**Purpose**: Apply a function to every element in a range.

**Syntax**: 
```cpp
std::for_each(first, last, unary_function);
```

**When to use**:
- Need to perform an operation on every element
- Side effects are intended (printing, modifying external state)
- More expressive than range-based for loop in some cases

**Example**:
```cpp
std::for_each(numbers.begin(), numbers.end(), 
              [](int n) { std::cout << n << " "; });
```

### 2. `std::transform`
**Purpose**: Create a new sequence by applying a transformation to each element.

**Syntax**: 
```cpp
std::transform(first, last, result, unary_op);
std::transform(first1, last1, first2, result, binary_op);
```

**When to use**:
- Need to convert elements from one type/value to another
- Creating a new container with transformed values
- Mathematical operations on sequences

**Example**:
```cpp
std::transform(numbers.begin(), numbers.end(), 
               std::back_inserter(squared),
               [](int n) { return n * n; });
```

### 3. `std::find_if`
**Purpose**: Find the first element that satisfies a condition.

**Syntax**: 
```cpp
auto it = std::find_if(first, last, predicate);
```

**When to use**:
- Search for first element meeting criteria
- Early termination when first match is sufficient
- Condition-based searching

**Example**:
```cpp
auto it = std::find_if(employees.begin(), employees.end(),
                       [](const Employee& e) { 
                           return e.salary > 80000; 
                       });
```

### 4. `std::count_if`
**Purpose**: Count how many elements satisfy a condition.

**Syntax**: 
```cpp
auto count = std::count_if(first, last, predicate);
```

**When to use**:
- Need to know quantity of elements meeting criteria
- Statistical analysis
- Validation (checking how many items are valid)

**Example**:
```cpp
auto count = std::count_if(sensors.begin(), sensors.end(),
                          [](const SensorData& s) { 
                              return s.is_valid && s.value > 50.0; 
                          });
```

### 5. Additional Powerful Algorithms

#### `std::copy_if`
Filter elements into a new container:
```cpp
std::copy_if(source.begin(), source.end(), 
             std::back_inserter(destination),
             [](const auto& item) { return condition; });
```

#### `std::accumulate`
Reduce a sequence to a single value:
```cpp
auto sum = std::accumulate(numbers.begin(), numbers.end(), 0);
auto product = std::accumulate(numbers.begin(), numbers.end(), 1,
                              [](int acc, int n) { return acc * n; });
```

#### `std::any_of` / `std::all_of` / `std::none_of`
Boolean logic on sequences:
```cpp
bool has_negative = std::any_of(numbers.begin(), numbers.end(),
                               [](int n) { return n < 0; });
bool all_positive = std::all_of(numbers.begin(), numbers.end(),
                               [](int n) { return n > 0; });
```

## Lambda Expressions Best Practices

### 1. Capture Modes
```cpp
[=]     // Capture all by value
[&]     // Capture all by reference  
[var]   // Capture var by value
[&var]  // Capture var by reference
[=, &var] // Capture all by value except var by reference
```

### 2. Parameter Types
```cpp
[](auto x) { return x * 2; }           // Generic lambda (C++14)
[](int x) { return x * 2; }            // Specific type
[](const auto& x) { return x.size(); } // Const reference for efficiency
```

### 3. Return Types
```cpp
[](int x) -> double { return x / 2.0; } // Explicit return type
[](int x) { return x * 2; }             // Deduced return type
```

## Performance Considerations

### When STL Algorithms Win:
1. **Compiler Optimizations**: Modern compilers can optimize STL algorithms better
2. **Vectorization**: Some algorithms can be auto-vectorized by compilers
3. **Cache Efficiency**: Algorithms often have better memory access patterns
4. **Less Error-Prone**: No manual index management

### When Traditional Loops Win:
1. **Complex Logic**: Multiple nested conditions might be clearer as loops
2. **Multiple Operations**: When you need to do several things in one pass
3. **Early Exit**: Complex break conditions
4. **Memory Constraints**: When lambda captures might use too much stack

## Embedded Systems Considerations

### Memory Safety:
- Prefer algorithms that work with existing containers
- Avoid algorithms that allocate memory (use `std::back_inserter` carefully)
- Use stack-based containers when possible

### Performance:
- Profile on actual target hardware
- Consider instruction cache pressure from template instantiation
- Use `-Os` or `-Oz` for size optimization if needed

### Debugging:
- Some embedded debuggers struggle with heavy template usage
- Consider keeping traditional loop versions for debugging builds

## Common Patterns and Idioms

### Filter-Transform-Reduce Pattern:
```cpp
// Traditional approach
std::vector<Result> results;
for (const auto& item : input) {
    if (meets_criteria(item)) {
        results.push_back(transform(item));
    }
}
auto final_result = reduce(results);

// STL approach
std::vector<Intermediate> filtered;
std::copy_if(input.begin(), input.end(), 
             std::back_inserter(filtered), meets_criteria);

std::vector<Result> transformed;
std::transform(filtered.begin(), filtered.end(),
               std::back_inserter(transformed), transform);

auto final_result = std::accumulate(transformed.begin(), transformed.end(),
                                   initial_value, reduce_op);
```

### In-Place Operations:
```cpp
// Sort in place
std::sort(data.begin(), data.end());

// Transform in place
std::transform(data.begin(), data.end(), data.begin(),
               [](auto& item) { return transform(item); });

// Remove elements (erase-remove idiom)
data.erase(std::remove_if(data.begin(), data.end(), should_remove),
           data.end());
```

## Build and Run Instructions

See [BUILD_AND_RUN.md](BUILD_AND_RUN.md) for detailed compilation and execution instructions.

## Learning Path

1. **Start Simple**: Begin with `std::for_each` on simple data types
2. **Add Complexity**: Move to `std::transform` with custom types
3. **Search and Count**: Master `std::find_if` and `std::count_if`
4. **Combine Algorithms**: Chain multiple algorithms together
5. **Optimize**: Compare performance and choose appropriate approach

## Next Steps

- Explore parallel algorithms (`std::execution::par`)
- Study ranges library (C++20)
- Learn about algorithm customization points
- Practice with real embedded system constraints

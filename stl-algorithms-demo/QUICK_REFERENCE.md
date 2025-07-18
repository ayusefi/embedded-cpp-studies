# C++ STL Algorithms & Lambda Quick Reference

## 🚀 Quick Start
```bash
cd stl-algorithms-demo
make run-all    # Build and run both demos
```

## 📚 Core Algorithms Cheat Sheet

### Data Processing
```cpp
// Apply function to all elements
std::for_each(begin, end, [](auto& x) { process(x); });

// Transform elements to new container
std::transform(begin, end, std::back_inserter(result), 
               [](const auto& x) { return transform(x); });

// Filter elements
std::copy_if(begin, end, std::back_inserter(result),
             [](const auto& x) { return condition(x); });
```

### Search & Count
```cpp
// Find first matching element
auto it = std::find_if(begin, end, [](const auto& x) { return condition(x); });

// Count matching elements
auto count = std::count_if(begin, end, [](const auto& x) { return condition(x); });

// Check existence
bool any = std::any_of(begin, end, [](const auto& x) { return condition(x); });
bool all = std::all_of(begin, end, [](const auto& x) { return condition(x); });
```

### Reduction
```cpp
// Sum all elements
auto sum = std::accumulate(begin, end, 0);

// Custom reduction
auto result = std::accumulate(begin, end, initial,
                             [](auto acc, const auto& x) { return combine(acc, x); });
```

## 🔧 Lambda Capture Quick Guide

| Syntax | Description | Use Case |
|--------|-------------|----------|
| `[]` | No capture | Pure functions |
| `[x]` | Capture `x` by value | Small, cheap-to-copy variables |
| `[&x]` | Capture `x` by reference | Large objects, need to modify |
| `[=]` | Capture all by value | When you need many variables |
| `[&]` | Capture all by reference | Performance critical, careful with lifetime |
| `[=, &x]` | Mixed capture | Most variables by value, `x` by reference |

## ⚡ Performance Tips for Embedded

### ✅ DO
- Use capture by value for primitive types
- Prefer algorithms that work with existing containers
- Use `constexpr` lambdas when possible
- Profile on actual target hardware

### ❌ AVOID
- Algorithms that allocate memory (`std::back_inserter` can grow vectors)
- Capturing large objects by value unnecessarily
- Complex nested lambda expressions
- Using `std::function` in performance-critical code

## 🎯 Common Patterns

### Filter → Transform → Reduce
```cpp
// Traditional approach
std::vector<ResultType> temp;
for (const auto& item : input) {
    if (filter_condition(item)) {
        temp.push_back(transform(item));
    }
}
auto result = reduce(temp);

// STL approach
std::vector<TransformedType> filtered_transformed;
std::for_each(input.begin(), input.end(),
              [&](const auto& item) {
                  if (filter_condition(item)) {
                      filtered_transformed.push_back(transform(item));
                  }
              });
auto result = std::accumulate(filtered_transformed.begin(), 
                             filtered_transformed.end(), 
                             initial_value, reduce_op);
```

### Embedded Sensor Processing
```cpp
// Process valid sensor readings
std::for_each(sensors.begin(), sensors.end(),
              [&results](const SensorReading& reading) {
                  if (reading.is_valid && reading.value > threshold) {
                      results.push_back(calibrate(reading.value));
                  }
              });
```

## 📊 Algorithm Performance Guide

| Algorithm | Time Complexity | Space | Use Case |
|-----------|----------------|-------|----------|
| `for_each` | O(n) | O(1) | Apply function to all |
| `transform` | O(n) | O(n) | Convert all elements |
| `find_if` | O(n) | O(1) | Find first match |
| `count_if` | O(n) | O(1) | Count matches |
| `copy_if` | O(n) | O(k) | Filter elements |
| `accumulate` | O(n) | O(1) | Reduce to single value |

## 🛠️ Build Targets

```bash
make all         # Build both demos
make run         # Run algorithms comparison
make run-lambda  # Run lambda deep dive
make run-all     # Run both demos
make debug       # Build debug version
make release     # Build optimized version
make embedded    # Build size-optimized
make clean       # Clean all artifacts
```

## 📁 Files Overview

- `algorithms_comparison.cpp` - Traditional vs STL comparison
- `lambda_deep_dive.cpp` - Comprehensive lambda tutorial
- `README.md` - Detailed study guide
- `LEARNING_SUMMARY.md` - Complete learning outcomes
- `BUILD_AND_RUN.md` - Build instructions
- `Makefile` - Build automation

## 🎓 Learning Path

1. **Start**: Run the demos to see examples
2. **Study**: Read `README.md` for algorithm details
3. **Practice**: Modify examples with your own data
4. **Experiment**: Try different lambda capture modes
5. **Optimize**: Profile and compare performance
6. **Apply**: Use in your embedded projects

## 🔗 Next Steps

- Explore parallel algorithms (`std::execution::par`)
- Learn C++20 ranges library
- Study custom allocators for embedded systems
- Practice with real-time constraints

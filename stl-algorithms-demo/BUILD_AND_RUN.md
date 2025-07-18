# Build and Run Instructions for STL Algorithms Demo

## Quick Start

### Option 1: Direct Compilation
```bash
cd stl-algorithms-demo
g++ -std=c++17 -O2 -Wall -Wextra algorithms_comparison.cpp -o algorithms_demo
./algorithms_demo
```

### Option 2: Using Makefile
```bash
cd stl-algorithms-demo
make all
make run
```

## Compilation Options

### Debug Build
```bash
g++ -std=c++17 -g -O0 -Wall -Wextra -DDEBUG algorithms_comparison.cpp -o algorithms_demo_debug
```

### Release Build with Optimizations
```bash
g++ -std=c++17 -O3 -DNDEBUG -march=native algorithms_comparison.cpp -o algorithms_demo_optimized
```

### Embedded Systems Build (ARM Cortex-M)
```bash
arm-none-eabi-g++ -std=c++17 -Os -mcpu=cortex-m4 -mthumb \
                  -fno-exceptions -fno-rtti \
                  algorithms_comparison.cpp -o algorithms_demo.elf
```

## Performance Analysis

### Timing Individual Functions
The demo includes built-in timing for performance comparison. To get more detailed analysis:

```bash
# Compile with profiling
g++ -std=c++17 -O2 -pg algorithms_comparison.cpp -o algorithms_demo_profile
./algorithms_demo_profile
gprof algorithms_demo_profile gmon.out > analysis.txt
```

### Memory Usage Analysis
```bash
# Compile with address sanitizer for memory analysis
g++ -std=c++17 -O1 -fsanitize=address -g algorithms_comparison.cpp -o algorithms_demo_asan
./algorithms_demo_asan
```

## Expected Output

The program will demonstrate:

1. **Even Numbers Squared Problem**: 
   - Traditional loop vs STL algorithm comparison
   - Performance timing

2. **Employee Data Processing**:
   - Complex filtering and transformation
   - Multiple algorithm usage

3. **Sensor Data Analysis**:
   - Focused demonstration of key algorithms
   - Real-world embedded systems example

4. **Advanced Algorithms**:
   - Sorting, partitioning, accumulation
   - Boolean logic operations

5. **Performance Comparison**:
   - Large dataset processing
   - Timing comparison between approaches

## Troubleshooting

### Compilation Errors

1. **C++17 not supported**: Use `-std=c++14` instead (some features may not work)
2. **Missing headers**: Ensure you have a complete standard library implementation
3. **Template instantiation errors**: Check compiler memory limits

### Runtime Issues

1. **Slow performance**: Try different optimization levels (-O1, -O2, -O3)
2. **Memory issues**: Reduce dataset sizes in performance comparison section
3. **Stack overflow**: Increase stack size or reduce lambda capture sizes

## Educational Exercises

### Exercise 1: Add More Algorithms
Extend the demo to include:
- `std::partition`
- `std::nth_element`  
- `std::unique`
- `std::merge`

### Exercise 2: Custom Comparators
Create examples using:
- Custom comparison functions
- Multiple sort criteria
- Stable vs unstable sorting

### Exercise 3: Embedded Constraints
Modify the code to work under embedded constraints:
- No dynamic allocation
- Fixed-size containers only
- Limited stack space

### Exercise 4: Performance Optimization
- Profile the code and identify bottlenecks
- Compare different lambda capture methods
- Test with different compiler optimization flags

## Integration with Embedded Projects

To use these patterns in embedded systems:

1. **Replace std::vector with std::array** for fixed-size containers
2. **Use custom allocators** for memory pools
3. **Avoid algorithms that allocate** memory dynamically
4. **Test on actual target hardware** for performance validation

## Compiler Support

| Compiler | Version | C++17 Support | Notes |
|----------|---------|---------------|-------|
| GCC      | 7+      | Full          | Recommended |
| Clang    | 5+      | Full          | Good alternative |
| MSVC     | 2017+   | Full          | Windows |
| ARM GCC  | 8+      | Full          | Embedded targets |

## Additional Resources

- [C++ Reference - Algorithms](https://en.cppreference.com/w/cpp/algorithm)
- [CppCon talks on STL algorithms](https://www.youtube.com/results?search_query=cppcon+stl+algorithms)
- [Effective Modern C++](https://www.oreilly.com/library/view/effective-modern-c/9781491908419/) - Items on lambdas and algorithms

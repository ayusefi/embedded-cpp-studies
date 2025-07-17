# C++ Templates: Complete Study Guide

## Core Concepts

### What are Templates?

Templates are a C++ feature that allows you to write **generic code** that works with multiple data types. They are the foundation of the Standard Template Library (STL) and modern C++ programming.

**Key Benefits:**
- **Code Reuse**: Write once, use with many types
- **Type Safety**: Compile-time type checking
- **Zero Runtime Overhead**: All resolution happens at compile time
- **Performance**: No virtual function calls or dynamic dispatch

## Function Templates

### Basic Syntax
```cpp
template <typename T>
T max_value(T a, T b) {
    return (a > b) ? a : b;
}
```

### How Function Templates Work
1. **Template Declaration**: `template <typename T>`
2. **Generic Implementation**: Use `T` as a placeholder type
3. **Compiler Instantiation**: Compiler generates specific versions for each type used
4. **Type Deduction**: Compiler automatically deduces types from arguments

### Example Usage
```cpp
int result1 = max_value(10, 20);        // T = int
double result2 = max_value(3.14, 2.71); // T = double
char result3 = max_value('A', 'Z');     // T = char
```

### Multiple Template Parameters
```cpp
template <typename T1, typename T2>
void print_pair(T1 first, T2 second) {
    std::cout << "(" << first << ", " << second << ")" << std::endl;
}

// Usage
print_pair(42, 3.14);           // T1=int, T2=double
print_pair("Age", 25);          // T1=const char*, T2=int
```

### Template Specialization
```cpp
// Generic version
template <typename T>
bool are_equal(T a, T b) {
    return a == b;
}

// Specialized version for floating point
template <>
bool are_equal<double>(double a, double b) {
    const double epsilon = 1e-9;
    return std::abs(a - b) < epsilon;
}
```

## Class Templates

### Basic Syntax
```cpp
template <typename T, typename U>
class Pair {
private:
    T first;
    U second;
    
public:
    Pair(const T& f, const U& s) : first(f), second(s) {}
    
    const T& getFirst() const { return first; }
    const U& getSecond() const { return second; }
};
```

### Template Instantiation
```cpp
Pair<int, double> p1(42, 3.14159);          // T=int, U=double
Pair<std::string, int> p2("Age", 25);       // T=string, U=int
Pair<char, std::string> p3('X', "marks");   // T=char, U=string
```

### Non-Type Template Parameters
```cpp
template <typename T, size_t Size>
class Array {
private:
    T data[Size];  // Size is known at compile time
    
public:
    constexpr size_t size() const { return Size; }
    T& operator[](size_t index) { return data[index]; }
};

// Usage
Array<int, 10> int_array;      // 10 integers
Array<double, 5> double_array; // 5 doubles
```

### Class Template Specialization
```cpp
// Generic vector
template <typename T>
class Vector { /* ... */ };

// Specialized version for bool (space-efficient)
template <>
class Vector<bool> {
    // Use bit packing for space efficiency
    // Implementation differs from generic version
};
```

## Advanced Template Features

### Variadic Templates (C++11)
```cpp
template <typename T>
void print_values(T&& value) {
    std::cout << value << std::endl;
}

template <typename T, typename... Args>
void print_values(T&& first, Args&&... args) {
    std::cout << first << " ";
    print_values(args...);  // Recursive call
}

// Usage
print_values(1, 2.5, "hello", 'X');  // Any number of arguments
```

### SFINAE (Substitution Failure Is Not An Error)
```cpp
template <typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type
safe_divide(T a, T b) {
    // Version for integer types
    return (b == 0) ? 0 : a / b;
}

template <typename T>
typename std::enable_if<std::is_floating_point<T>::value, T>::type
safe_divide(T a, T b) {
    // Version for floating point types
    return (b == 0.0) ? std::numeric_limits<T>::infinity() : a / b;
}
```

### Template Metaprogramming
```cpp
// Compile-time factorial calculation
template <int N>
struct Factorial {
    static constexpr int value = N * Factorial<N-1>::value;
};

template <>
struct Factorial<0> {
    static constexpr int value = 1;
};

// Usage
constexpr int fact5 = Factorial<5>::value;  // Calculated at compile time
```

## Templates in Embedded Systems

### Fixed-Point Arithmetic
```cpp
template <typename BaseType, int FractionalBits>
class FixedPoint {
private:
    BaseType value;
    static constexpr BaseType SCALE = 1 << FractionalBits;
    
public:
    FixedPoint(float f) : value(static_cast<BaseType>(f * SCALE)) {}
    float to_float() const { return static_cast<float>(value) / SCALE; }
    
    FixedPoint operator+(const FixedPoint& other) const {
        FixedPoint result;
        result.value = value + other.value;
        return result;
    }
};

// Usage
using Fixed16_8 = FixedPoint<int16_t, 8>;  // 16-bit, 8 fractional bits
Fixed16_8 a(3.14f);
Fixed16_8 b(2.5f);
Fixed16_8 sum = a + b;  // No floating point operations!
```

### Type-Safe GPIO
```cpp
template <uint8_t PinNumber>
class GPIO {
private:
    static constexpr uint8_t pin = PinNumber;
    
public:
    GPIO() {
        static_assert(PinNumber < 32, "Pin number must be less than 32");
    }
    
    void set_high() { /* Set pin high */ }
    void set_low() { /* Set pin low */ }
    constexpr uint8_t get_pin_number() const { return pin; }
};

// Usage
GPIO<13> led;     // LED on pin 13
GPIO<2> button;   // Button on pin 2
// GPIO<50> invalid; // Compile-time error!
```

### Memory Pool Template
```cpp
template <typename T, size_t PoolSize>
class MemoryPool {
private:
    alignas(T) uint8_t pool[PoolSize * sizeof(T)];
    bool used[PoolSize];
    
public:
    T* allocate() {
        // Find free slot and return pointer
        // Deterministic allocation, no heap fragmentation
    }
    
    void deallocate(T* ptr) {
        // Mark slot as free
    }
};

// Usage
MemoryPool<SensorReading, 10> sensor_pool;  // Pool for 10 sensor readings
SensorReading* reading = sensor_pool.allocate();  // Fast, deterministic
```

### Ring Buffer Template
```cpp
template <typename T, size_t Size>
class RingBuffer {
private:
    T buffer[Size];
    size_t head, tail;
    bool full;
    
public:
    bool push(const T& item) { /* Add to buffer */ }
    bool pop(T& item) { /* Remove from buffer */ }
    bool empty() const { return (!full && (head == tail)); }
    constexpr size_t capacity() const { return Size; }
};

// Usage
RingBuffer<uint8_t, 256> uart_buffer;  // UART receive buffer
RingBuffer<float, 100> sensor_buffer;  // Sensor data buffer
```

## Key Benefits for Embedded Systems

### 1. Zero Runtime Overhead
- All template instantiation happens at compile time
- No virtual function calls or dynamic dispatch
- Direct, optimized machine code generation

### 2. Type Safety
- `GPIO<13>` vs `GPIO<14>` are different types
- Compile-time error if you mix them up
- Prevents runtime errors in critical systems

### 3. Memory Efficiency
- Fixed-size containers with compile-time bounds checking
- Memory pools eliminate dynamic allocation
- Ring buffers provide predictable memory usage

### 4. Performance Predictability
- All operations have known time complexity
- No garbage collection or hidden allocations
- Suitable for real-time systems

### 5. Code Reuse
- Same template works for different data types
- Reduces code duplication and maintenance
- Easier to test and validate

## Template Best Practices

### 1. Use Meaningful Names
```cpp
// Good
template <typename ElementType, size_t MaxElements>
class CircularBuffer { /* ... */ };

// Bad
template <typename T, size_t N>
class CB { /* ... */ };
```

### 2. Provide Clear Error Messages
```cpp
template <typename T, size_t Size>
class Array {
    static_assert(Size > 0, "Array size must be greater than 0");
    static_assert(std::is_default_constructible<T>::value, 
                  "Array element type must be default constructible");
};
```

### 3. Use Template Specialization When Needed
```cpp
// Generic version for most types
template <typename T>
class Serializer { /* ... */ };

// Specialized version for complex types
template <>
class Serializer<MyComplexType> { /* Custom serialization */ };
```

### 4. Consider Template Constraints (C++20)
```cpp
template <typename T>
requires std::integral<T>
T safe_add(T a, T b) {
    // Only works with integral types
    return a + b;
}
```

## STL Connection

Understanding templates helps you understand the STL:

```cpp
std::vector<int>           // Vector template instantiated with int
std::map<string, int>      // Map template with string keys, int values
std::unique_ptr<MyClass>   // Smart pointer template
std::function<void(int)>   // Function wrapper template
```

All STL containers and algorithms are templates, providing:
- Type safety
- Performance
- Reusability

## Common Template Patterns

### 1. CRTP (Curiously Recurring Template Pattern)
```cpp
template <typename Derived>
class Base {
public:
    void interface() {
        static_cast<Derived*>(this)->implementation();
    }
};

class Derived : public Base<Derived> {
public:
    void implementation() { /* ... */ }
};
```

### 2. Tag Dispatching
```cpp
struct IntegralTag {};
struct FloatingTag {};

template <typename T>
void process_impl(T value, IntegralTag) {
    // Implementation for integral types
}

template <typename T>
void process_impl(T value, FloatingTag) {
    // Implementation for floating point types
}

template <typename T>
void process(T value) {
    using Tag = std::conditional_t<std::is_integral_v<T>, IntegralTag, FloatingTag>;
    process_impl(value, Tag{});
}
```

### 3. Policy-Based Design
```cpp
template <typename StoragePolicy, typename ValidationPolicy>
class ConfigManager : private StoragePolicy, private ValidationPolicy {
public:
    void set_value(const std::string& key, const std::string& value) {
        if (ValidationPolicy::validate(value)) {
            StoragePolicy::store(key, value);
        }
    }
};
```

## Debugging Templates

### 1. Understanding Error Messages
Template errors can be verbose. Focus on:
- The template instantiation chain
- The specific type that caused the failure
- Missing member functions or operators

### 2. Using Static Assertions
```cpp
template <typename T>
void my_function(T value) {
    static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type");
    // Function implementation
}
```

### 3. Template Debugging Tools
- Use `decltype` to inspect types
- Use `std::is_same` to check type equality
- Use compiler intrinsics like `__PRETTY_FUNCTION__`

## Summary

Templates are one of the most powerful features in C++, especially valuable for embedded systems programming:

1. **Write generic, reusable code** without sacrificing performance
2. **Achieve type safety** through compile-time checking
3. **Eliminate runtime overhead** through compile-time instantiation
4. **Create efficient, maintainable code** suitable for resource-constrained environments

Understanding templates is essential for:
- Using the STL effectively
- Writing modern, efficient C++ code
- Developing robust embedded systems
- Creating reusable, type-safe libraries

The examples in this project demonstrate practical applications of templates, from basic function templates to advanced embedded systems patterns.

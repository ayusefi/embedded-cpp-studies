#include <iostream>
#include <string>
#include <memory>
#include <type_traits>
#include <limits>

// ===== ADVANCED FUNCTION TEMPLATES =====

// Variadic template function (C++11)
template <typename T>
void print_values(T&& value) {
    std::cout << value << std::endl;
}

template <typename T, typename... Args>
void print_values(T&& first, Args&&... args) {
    std::cout << first << " ";
    print_values(args...);  // Recursive call
}

// SFINAE (Substitution Failure Is Not An Error) example
template <typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type
safe_divide(T a, T b) {
    if (b == 0) {
        std::cout << "Warning: Division by zero for integer types, returning 0\n";
        return 0;
    }
    return a / b;
}

template <typename T>
typename std::enable_if<std::is_floating_point<T>::value, T>::type
safe_divide(T a, T b) {
    if (b == 0.0) {
        std::cout << "Warning: Division by zero for floating point, returning infinity\n";
        return std::numeric_limits<T>::infinity();
    }
    return a / b;
}

// Template with auto return type (C++14)
template <typename T, typename U>
auto advanced_multiply(T a, U b) -> decltype(a * b) {
    std::cout << "Multiplying " << typeid(T).name() << " and " << typeid(U).name() << std::endl;
    return a * b;
}

// Perfect forwarding example
template <typename T>
T create_and_initialize(T&& value) {
    std::cout << "Creating object with perfect forwarding\n";
    return T(std::forward<T>(value));
}

// ===== TEMPLATE METAPROGRAMMING =====

// Compile-time factorial calculation
template <int N>
struct Factorial {
    static constexpr int value = N * Factorial<N-1>::value;
};

// Template specialization for base case
template <>
struct Factorial<0> {
    static constexpr int value = 1;
};

// Type traits example
template <typename T>
void analyze_type(T value) {
    std::cout << "Type analysis for: " << typeid(T).name() << std::endl;
    std::cout << "  Is integral: " << std::is_integral<T>::value << std::endl;
    std::cout << "  Is floating point: " << std::is_floating_point<T>::value << std::endl;
    std::cout << "  Is pointer: " << std::is_pointer<T>::value << std::endl;
    std::cout << "  Is const: " << std::is_const<T>::value << std::endl;
    std::cout << "  Size: " << sizeof(T) << " bytes" << std::endl;
    std::cout << "  Value: " << value << std::endl;
}

// ===== ADVANCED CLASS TEMPLATES =====

// Template with multiple parameter types and default parameters
template <typename T, typename Allocator = std::allocator<T>, size_t InitialCapacity = 16>
class AdvancedVector {
private:
    T* data;
    size_t current_size;
    size_t capacity;
    Allocator alloc;
    
public:
    AdvancedVector() : data(nullptr), current_size(0), capacity(InitialCapacity) {
        data = alloc.allocate(capacity);
        std::cout << "AdvancedVector created with capacity " << capacity << std::endl;
    }
    
    ~AdvancedVector() {
        for (size_t i = 0; i < current_size; ++i) {
            alloc.destroy(data + i);
        }
        alloc.deallocate(data, capacity);
    }
    
    void push_back(const T& value) {
        if (current_size >= capacity) {
            resize();
        }
        alloc.construct(data + current_size, value);
        ++current_size;
    }
    
    T& operator[](size_t index) { return data[index]; }
    const T& operator[](size_t index) const { return data[index]; }
    
    size_t size() const { return current_size; }
    size_t get_capacity() const { return capacity; }
    
    void display() const {
        std::cout << "AdvancedVector[" << current_size << "/" << capacity << "]: [";
        for (size_t i = 0; i < current_size; ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << data[i];
        }
        std::cout << "]" << std::endl;
    }
    
private:
    void resize() {
        size_t new_capacity = capacity * 2;
        T* new_data = alloc.allocate(new_capacity);
        
        for (size_t i = 0; i < current_size; ++i) {
            alloc.construct(new_data + i, std::move(data[i]));
            alloc.destroy(data + i);
        }
        
        alloc.deallocate(data, capacity);
        data = new_data;
        capacity = new_capacity;
        std::cout << "Resized to capacity " << capacity << std::endl;
    }
};

// Template specialization for bool (space-efficient)
template <typename Allocator, size_t InitialCapacity>
class AdvancedVector<bool, Allocator, InitialCapacity> {
private:
    uint8_t* data;
    size_t current_size;
    size_t capacity_bits;
    
public:
    AdvancedVector() : data(nullptr), current_size(0), capacity_bits(InitialCapacity) {
        size_t bytes_needed = (capacity_bits + 7) / 8;  // Round up to nearest byte
        data = new uint8_t[bytes_needed]();  // Zero-initialize
        std::cout << "Specialized bool AdvancedVector created with " << capacity_bits << " bit capacity" << std::endl;
    }
    
    ~AdvancedVector() {
        delete[] data;
    }
    
    void push_back(bool value) {
        if (current_size >= capacity_bits) {
            resize();
        }
        
        size_t byte_index = current_size / 8;
        size_t bit_index = current_size % 8;
        
        if (value) {
            data[byte_index] |= (1 << bit_index);
        } else {
            data[byte_index] &= ~(1 << bit_index);
        }
        
        ++current_size;
    }
    
    bool operator[](size_t index) const {
        size_t byte_index = index / 8;
        size_t bit_index = index % 8;
        return (data[byte_index] & (1 << bit_index)) != 0;
    }
    
    size_t size() const { return current_size; }
    size_t get_capacity() const { return capacity_bits; }
    
    void display() const {
        std::cout << "Bool AdvancedVector[" << current_size << "/" << capacity_bits << "]: [";
        for (size_t i = 0; i < current_size; ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << ((*this)[i] ? "true" : "false");
        }
        std::cout << "] (space-efficient: " << ((capacity_bits + 7) / 8) << " bytes)" << std::endl;
    }
    
private:
    void resize() {
        size_t new_capacity = capacity_bits * 2;
        size_t old_bytes = (capacity_bits + 7) / 8;
        size_t new_bytes = (new_capacity + 7) / 8;
        
        uint8_t* new_data = new uint8_t[new_bytes]();
        std::copy(data, data + old_bytes, new_data);
        
        delete[] data;
        data = new_data;
        capacity_bits = new_capacity;
        std::cout << "Bool vector resized to " << capacity_bits << " bits" << std::endl;
    }
};

// Template with non-type parameters
template <typename T, T DefaultValue, size_t Size>
class DefaultArray {
private:
    T data[Size];
    
public:
    DefaultArray() {
        std::fill(data, data + Size, DefaultValue);
        std::cout << "DefaultArray initialized with default value: " << DefaultValue << std::endl;
    }
    
    T& operator[](size_t index) { return data[index]; }
    const T& operator[](size_t index) const { return data[index]; }
    constexpr size_t size() const { return Size; }
    
    void display() const {
        std::cout << "DefaultArray[" << Size << "]: [";
        for (size_t i = 0; i < Size; ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << data[i];
        }
        std::cout << "]" << std::endl;
    }
};

// ===== DEMONSTRATION FUNCTIONS =====

void demonstrateVariadicTemplates() {
    std::cout << "=== Variadic Templates ===\n\n";
    
    std::cout << "print_values with different argument counts:\n";
    print_values(42);
    print_values(1, 2, 3);
    print_values("Hello", "Template", "World", "!");
    print_values(1, 2.5, "mixed", 'X', true);
    std::cout << "\n";
}

void demonstrateSFINAE() {
    std::cout << "=== SFINAE (Enable If) ===\n\n";
    
    std::cout << "Type-safe division:\n";
    std::cout << "safe_divide(10, 3) = " << safe_divide(10, 3) << std::endl;
    std::cout << "safe_divide(10, 0) = " << safe_divide(10, 0) << std::endl;
    std::cout << "safe_divide(10.0, 3.0) = " << safe_divide(10.0, 3.0) << std::endl;
    std::cout << "safe_divide(10.0, 0.0) = " << safe_divide(10.0, 0.0) << std::endl;
    std::cout << "\n";
}

void demonstrateAutoReturn() {
    std::cout << "=== Auto Return Types ===\n\n";
    
    auto result1 = advanced_multiply(5, 10);        // int * int
    auto result2 = advanced_multiply(3.14, 2);      // double * int
    auto result3 = advanced_multiply(2.5f, 4.0);    // float * double
    
    std::cout << "Results: " << result1 << ", " << result2 << ", " << result3 << std::endl;
    std::cout << "\n";
}

void demonstrateMetaprogramming() {
    std::cout << "=== Template Metaprogramming ===\n\n";
    
    std::cout << "Compile-time factorial calculations:\n";
    std::cout << "Factorial<5>::value = " << Factorial<5>::value << std::endl;
    std::cout << "Factorial<10>::value = " << Factorial<10>::value << std::endl;
    std::cout << "\n";
    
    std::cout << "Type analysis:\n";
    analyze_type(42);
    std::cout << "\n";
    analyze_type(3.14159);
    std::cout << "\n";
    
    const char* ptr = "Hello";
    analyze_type(ptr);
    std::cout << "\n";
}

void demonstrateAdvancedClasses() {
    std::cout << "=== Advanced Class Templates ===\n\n";
    
    std::cout << "1. AdvancedVector with custom capacity:\n";
    AdvancedVector<int, std::allocator<int>, 4> vec;
    
    for (int i = 1; i <= 6; ++i) {
        vec.push_back(i * 10);
        vec.display();
    }
    std::cout << "\n";
    
    std::cout << "2. Specialized bool vector (space-efficient):\n";
    AdvancedVector<bool, std::allocator<bool>, 8> bool_vec;
    
    bool values[] = {true, false, true, true, false, false, true, false, true, true};
    for (bool val : values) {
        bool_vec.push_back(val);
    }
    bool_vec.display();
    std::cout << "\n";
    
    std::cout << "3. DefaultArray with non-type parameters:\n";
    DefaultArray<int, 42, 5> default_int_array;
    default_int_array.display();
    
    DefaultArray<char, 'X', 8> default_char_array;
    default_char_array.display();
    
    // Modify some values
    default_int_array[2] = 100;
    default_char_array[3] = 'Y';
    default_int_array.display();
    default_char_array.display();
    std::cout << "\n";
}

int main() {
    std::cout << "Advanced C++ Templates Demonstration\n";
    std::cout << "====================================\n\n";
    
    demonstrateVariadicTemplates();
    std::cout << std::string(60, '-') << "\n\n";
    
    demonstrateSFINAE();
    std::cout << std::string(60, '-') << "\n\n";
    
    demonstrateAutoReturn();
    std::cout << std::string(60, '-') << "\n\n";
    
    demonstrateMetaprogramming();
    std::cout << std::string(60, '-') << "\n\n";
    
    demonstrateAdvancedClasses();
    
    return 0;
}

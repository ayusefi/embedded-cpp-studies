#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

// ===== BASIC FUNCTION TEMPLATES =====

// Simple generic print function
template <typename T>
void print_value(T value) {
    std::cout << "Value: " << value << std::endl;
}

// Generic max function for comparable types
template <typename T>
T max_value(T a, T b) {
    return (a > b) ? a : b;
}

// Generic min function
template <typename T>
T min_value(T a, T b) {
    return (a < b) ? a : b;
}

// Generic swap function
template <typename T>
void swap_values(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

// ===== MULTIPLE TEMPLATE PARAMETERS =====

// Function with two different template types
template <typename T1, typename T2>
void print_pair(T1 first, T2 second) {
    std::cout << "Pair: (" << first << ", " << second << ")" << std::endl;
}

// Generic conversion function
template <typename From, typename To>
To convert(From value) {
    return static_cast<To>(value);
}

// ===== TEMPLATE FUNCTIONS WITH CONTAINERS =====

// Generic function to print any container
template <typename Container>
void print_container(const Container& container, const std::string& label = "Container") {
    std::cout << label << ": [";
    bool first = true;
    for (const auto& item : container) {
        if (!first) std::cout << ", ";
        std::cout << item;
        first = false;
    }
    std::cout << "]" << std::endl;
}

// Generic function to find element in container
template <typename Container, typename T>
bool contains(const Container& container, const T& value) {
    for (const auto& item : container) {
        if (item == value) {
            return true;
        }
    }
    return false;
}

// Generic function to calculate sum
template <typename Container>
auto sum_container(const Container& container) {
    using ValueType = std::remove_const_t<std::remove_reference_t<decltype(*container.begin())>>;
    ValueType result{}; // Default initialize to zero
    for (const auto& item : container) {
        result += item;
    }
    return result;
}

// ===== FUNCTION TEMPLATE WITH DEFAULT PARAMETERS =====

// Function with default template parameter
template <typename T = int>
T get_default_value() {
    return T{};  // Default constructor
}

// ===== SPECIALIZED FUNCTION TEMPLATES =====

// Generic compare function
template <typename T>
bool are_equal(T a, T b) {
    return a == b;
}

// Specialized version for floating point comparison
template <>
bool are_equal<double>(double a, double b) {
    const double epsilon = 1e-9;
    return std::abs(a - b) < epsilon;
}

template <>
bool are_equal<float>(float a, float b) {
    const float epsilon = 1e-6f;
    return std::abs(a - b) < epsilon;
}

// ===== DEMONSTRATION FUNCTIONS =====

void demonstrateBasicTemplates() {
    std::cout << "=== Basic Function Templates ===\n\n";
    
    // Test print_value with different types
    std::cout << "1. Generic print_value function:\n";
    print_value(42);              // int
    print_value(3.14159);         // double
    print_value(std::string("Hello Templates!")); // string
    print_value('A');             // char
    std::cout << "\n";
    
    // Test max_value with different types
    std::cout << "2. Generic max_value function:\n";
    std::cout << "max(10, 20) = " << max_value(10, 20) << std::endl;
    std::cout << "max(3.14, 2.71) = " << max_value(3.14, 2.71) << std::endl;
    std::cout << "max('A', 'Z') = " << max_value('A', 'Z') << std::endl;
    std::cout << "max(\"apple\", \"banana\") = " << max_value(std::string("apple"), std::string("banana")) << std::endl;
    std::cout << "\n";
    
    // Test swap_values
    std::cout << "3. Generic swap_values function:\n";
    int x = 100, y = 200;
    std::cout << "Before swap: x = " << x << ", y = " << y << std::endl;
    swap_values(x, y);
    std::cout << "After swap:  x = " << x << ", y = " << y << std::endl;
    
    std::string s1 = "World", s2 = "Hello";
    std::cout << "Before swap: s1 = \"" << s1 << "\", s2 = \"" << s2 << "\"" << std::endl;
    swap_values(s1, s2);
    std::cout << "After swap:  s1 = \"" << s1 << "\", s2 = \"" << s2 << "\"" << std::endl;
    std::cout << "\n";
}

void demonstrateMultipleParameters() {
    std::cout << "=== Multiple Template Parameters ===\n\n";
    
    std::cout << "1. print_pair with different type combinations:\n";
    print_pair(42, 3.14159);           // int, double
    print_pair(std::string("Age"), 25); // string, int
    print_pair('X', std::string("marks the spot")); // char, string
    std::cout << "\n";
    
    std::cout << "2. Type conversion examples:\n";
    double d = 3.14159;
    int i = convert<double, int>(d);
    std::cout << "convert<double, int>(" << d << ") = " << i << std::endl;
    
    float f = convert<int, float>(42);
    std::cout << "convert<int, float>(42) = " << f << std::endl;
    
    char c = convert<int, char>(65);
    std::cout << "convert<int, char>(65) = '" << c << "'" << std::endl;
    std::cout << "\n";
}

void demonstrateContainerTemplates() {
    std::cout << "=== Container Function Templates ===\n\n";
    
    // Test with different container types
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    std::vector<std::string> words = {"Hello", "Template", "World"};
    std::vector<double> decimals = {1.1, 2.2, 3.3, 4.4};
    
    std::cout << "1. Generic container printing:\n";
    print_container(numbers, "Numbers");
    print_container(words, "Words");
    print_container(decimals, "Decimals");
    std::cout << "\n";
    
    std::cout << "2. Generic contains function:\n";
    std::cout << "Numbers contains 3: " << (contains(numbers, 3) ? "Yes" : "No") << std::endl;
    std::cout << "Numbers contains 10: " << (contains(numbers, 10) ? "Yes" : "No") << std::endl;
    std::cout << "Words contains \"Template\": " << (contains(words, std::string("Template")) ? "Yes" : "No") << std::endl;
    std::cout << "\n";
    
    std::cout << "3. Generic sum calculation:\n";
    std::cout << "Sum of numbers: " << sum_container(numbers) << std::endl;
    std::cout << "Sum of decimals: " << std::fixed << std::setprecision(2) << sum_container(decimals) << std::endl;
    std::cout << "\n";
}

void demonstrateSpecialization() {
    std::cout << "=== Template Specialization ===\n\n";
    
    std::cout << "1. Integer comparison (exact):\n";
    std::cout << "are_equal(10, 10): " << (are_equal(10, 10) ? "true" : "false") << std::endl;
    std::cout << "are_equal(10, 11): " << (are_equal(10, 11) ? "true" : "false") << std::endl;
    std::cout << "\n";
    
    std::cout << "2. Floating point comparison (with epsilon):\n";
    double a = 0.1 + 0.2;
    double b = 0.3;
    std::cout << std::fixed << std::setprecision(20);
    std::cout << "a = 0.1 + 0.2 = " << a << std::endl;
    std::cout << "b = 0.3       = " << b << std::endl;
    std::cout << "a == b (exact): " << (a == b ? "true" : "false") << std::endl;
    std::cout << "are_equal<double>(a, b): " << (are_equal(a, b) ? "true" : "false") << std::endl;
    std::cout << "\n";
    
    std::cout << "3. Default template parameters:\n";
    std::cout << "get_default_value<int>(): " << get_default_value<int>() << std::endl;
    std::cout << "get_default_value<double>(): " << get_default_value<double>() << std::endl;
    std::cout << "get_default_value<std::string>(): \"" << get_default_value<std::string>() << "\"" << std::endl;
    std::cout << "get_default_value() (defaults to int): " << get_default_value() << std::endl;
    std::cout << "\n";
}

void demonstrateCompilerGeneration() {
    std::cout << "=== Understanding Template Instantiation ===\n\n";
    
    std::cout << "When you call a template function with different types,\n";
    std::cout << "the compiler generates separate functions for each type:\n\n";
    
    std::cout << "max_value(10, 20)       -> generates max_value<int>(int, int)\n";
    std::cout << "max_value(3.14, 2.71)   -> generates max_value<double>(double, double)\n";
    std::cout << "max_value('A', 'Z')     -> generates max_value<char>(char, char)\n\n";
    
    std::cout << "This happens at compile time, so there's no runtime overhead!\n";
    std::cout << "Each instantiation is a separate, optimized function.\n\n";
}

int main() {
    std::cout << "C++ Function Templates Demonstration\n";
    std::cout << "====================================\n\n";
    
    demonstrateBasicTemplates();
    std::cout << std::string(60, '-') << "\n\n";
    
    demonstrateMultipleParameters();
    std::cout << std::string(60, '-') << "\n\n";
    
    demonstrateContainerTemplates();
    std::cout << std::string(60, '-') << "\n\n";
    
    demonstrateSpecialization();
    std::cout << std::string(60, '-') << "\n\n";
    
    demonstrateCompilerGeneration();
    
    return 0;
}

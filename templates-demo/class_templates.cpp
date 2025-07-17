#include <iostream>
#include <string>
#include <vector>
#include <utility>

// ===== BASIC CLASS TEMPLATE =====

// Generic Pair class template
template <typename T, typename U>
class Pair {
private:
    T first;
    U second;

public:
    // Constructor
    Pair(const T& f, const U& s) : first(f), second(s) {
        std::cout << "Pair constructor called\n";
    }
    
    // Copy constructor
    Pair(const Pair& other) : first(other.first), second(other.second) {
        std::cout << "Pair copy constructor called\n";
    }
    
    // Move constructor
    Pair(Pair&& other) noexcept : first(std::move(other.first)), second(std::move(other.second)) {
        std::cout << "Pair move constructor called\n";
    }
    
    // Getters
    const T& getFirst() const { return first; }
    const U& getSecond() const { return second; }
    
    // Setters
    void setFirst(const T& value) { first = value; }
    void setSecond(const U& value) { second = value; }
    
    // Display method
    void display() const {
        std::cout << "Pair(" << first << ", " << second << ")" << std::endl;
    }
    
    // Comparison operators
    bool operator==(const Pair& other) const {
        return first == other.first && second == other.second;
    }
    
    bool operator!=(const Pair& other) const {
        return !(*this == other);
    }
};

// ===== SIMPLE CONTAINER CLASS TEMPLATE =====

// Generic Array class template
template <typename T, size_t Size>
class Array {
private:
    T data[Size];
    
public:
    // Constructor
    Array() {
        std::cout << "Array<" << typeid(T).name() << ", " << Size << "> constructor\n";
        // Initialize with default values
        for (size_t i = 0; i < Size; ++i) {
            data[i] = T{};
        }
    }
    
    // Constructor with initial value
    Array(const T& initial_value) {
        std::cout << "Array constructor with initial value\n";
        for (size_t i = 0; i < Size; ++i) {
            data[i] = initial_value;
        }
    }
    
    // Access operators
    T& operator[](size_t index) {
        if (index >= Size) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }
    
    const T& operator[](size_t index) const {
        if (index >= Size) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }
    
    // Size getter
    constexpr size_t size() const { return Size; }
    
    // Fill array with value
    void fill(const T& value) {
        for (size_t i = 0; i < Size; ++i) {
            data[i] = value;
        }
    }
    
    // Display array contents
    void display() const {
        std::cout << "Array[" << Size << "]: [";
        for (size_t i = 0; i < Size; ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << data[i];
        }
        std::cout << "]" << std::endl;
    }
    
    // Iterator support (basic)
    T* begin() { return data; }
    T* end() { return data + Size; }
    const T* begin() const { return data; }
    const T* end() const { return data + Size; }
};

// ===== STACK CLASS TEMPLATE =====

template <typename T, size_t MaxSize = 100>
class Stack {
private:
    T data[MaxSize];
    size_t top_index;
    
public:
    Stack() : top_index(0) {
        std::cout << "Stack<" << typeid(T).name() << ", " << MaxSize << "> created\n";
    }
    
    // Push element
    void push(const T& value) {
        if (top_index >= MaxSize) {
            throw std::overflow_error("Stack overflow");
        }
        data[top_index++] = value;
    }
    
    // Pop element
    T pop() {
        if (empty()) {
            throw std::underflow_error("Stack underflow");
        }
        return data[--top_index];
    }
    
    // Peek at top element
    const T& top() const {
        if (empty()) {
            throw std::underflow_error("Stack is empty");
        }
        return data[top_index - 1];
    }
    
    // Check if empty
    bool empty() const {
        return top_index == 0;
    }
    
    // Get current size
    size_t size() const {
        return top_index;
    }
    
    // Get maximum capacity
    constexpr size_t capacity() const {
        return MaxSize;
    }
    
    // Display stack contents
    void display() const {
        std::cout << "Stack (bottom to top): [";
        for (size_t i = 0; i < top_index; ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << data[i];
        }
        std::cout << "]" << std::endl;
    }
};

// ===== SPECIALIZED CLASS TEMPLATE =====

// Generic Calculator class
template <typename T>
class Calculator {
public:
    static T add(const T& a, const T& b) {
        return a + b;
    }
    
    static T subtract(const T& a, const T& b) {
        return a - b;
    }
    
    static T multiply(const T& a, const T& b) {
        return a * b;
    }
    
    static T divide(const T& a, const T& b) {
        return a / b;
    }
};

// Specialized version for string concatenation
template <>
class Calculator<std::string> {
public:
    static std::string add(const std::string& a, const std::string& b) {
        return a + b;  // String concatenation
    }
    
    static std::string subtract(const std::string& a, const std::string& b) {
        // Remove all occurrences of b from a
        std::string result = a;
        size_t pos = 0;
        while ((pos = result.find(b, pos)) != std::string::npos) {
            result.erase(pos, b.length());
        }
        return result;
    }
    
    static std::string multiply(const std::string& a, const std::string& b) {
        // Repeat string a, b times (if b can be converted to int)
        try {
            int times = std::stoi(b);
            std::string result;
            for (int i = 0; i < times; ++i) {
                result += a;
            }
            return result;
        } catch (...) {
            return "Error: Cannot multiply strings";
        }
    }
    
    static std::string divide(const std::string& a, const std::string& b) {
        return "String division not supported";
    }
};

// ===== DEMONSTRATION FUNCTIONS =====

void demonstratePairClass() {
    std::cout << "=== Pair Class Template ===\n\n";
    
    std::cout << "1. Creating different Pair instances:\n";
    Pair<int, double> p1(42, 3.14159);
    p1.display();
    
    Pair<std::string, int> p2("Age", 25);
    p2.display();
    
    Pair<char, std::string> p3('X', "marks the spot");
    p3.display();
    
    // Complex nested types
    std::vector<int> vec = {1, 2, 3, 4, 5};
    Pair<std::string, std::vector<int>> p4("Numbers", vec);
    std::cout << "Complex pair: " << p4.getFirst() << " with vector of size " << p4.getSecond().size() << std::endl;
    std::cout << "\n";
    
    std::cout << "2. Using getters and setters:\n";
    std::cout << "p1.getFirst() = " << p1.getFirst() << std::endl;
    std::cout << "p1.getSecond() = " << p1.getSecond() << std::endl;
    
    p1.setFirst(100);
    p1.setSecond(2.71828);
    std::cout << "After modification: ";
    p1.display();
    std::cout << "\n";
    
    std::cout << "3. Comparison operators:\n";
    Pair<int, double> p5(100, 2.71828);
    std::cout << "p1 == p5: " << (p1 == p5 ? "true" : "false") << std::endl;
    
    Pair<int, double> p6(50, 1.41421);
    std::cout << "p1 != p6: " << (p1 != p6 ? "true" : "false") << std::endl;
    std::cout << "\n";
}

void demonstrateArrayClass() {
    std::cout << "=== Array Class Template ===\n\n";
    
    std::cout << "1. Creating different Array types:\n";
    Array<int, 5> int_array;
    Array<double, 3> double_array(2.5);  // Initialize with 2.5
    Array<std::string, 4> string_array;
    
    std::cout << "\n2. Filling and displaying arrays:\n";
    // Fill int array
    for (size_t i = 0; i < int_array.size(); ++i) {
        int_array[i] = static_cast<int>(i * 10);
    }
    int_array.display();
    
    // Display double array
    double_array.display();
    
    // Fill string array
    string_array[0] = "Hello";
    string_array[1] = "Template";
    string_array[2] = "World";
    string_array[3] = "!";
    string_array.display();
    
    std::cout << "\n3. Using fill method:\n";
    Array<char, 6> char_array;
    char_array.fill('*');
    char_array.display();
    std::cout << "\n";
}

void demonstrateStackClass() {
    std::cout << "=== Stack Class Template ===\n\n";
    
    std::cout << "1. Integer stack operations:\n";
    Stack<int, 10> int_stack;
    
    // Push elements
    for (int i = 1; i <= 5; ++i) {
        int_stack.push(i * 10);
        std::cout << "Pushed: " << (i * 10) << std::endl;
    }
    
    int_stack.display();
    std::cout << "Stack size: " << int_stack.size() << "/" << int_stack.capacity() << std::endl;
    
    // Pop elements
    std::cout << "\nPopping elements:\n";
    while (!int_stack.empty()) {
        std::cout << "Popped: " << int_stack.pop() << std::endl;
        int_stack.display();
    }
    std::cout << "\n";
    
    std::cout << "2. String stack operations:\n";
    Stack<std::string, 5> string_stack;
    
    string_stack.push("First");
    string_stack.push("Second");
    string_stack.push("Third");
    
    string_stack.display();
    std::cout << "Top element: " << string_stack.top() << std::endl;
    std::cout << "\n";
}

void demonstrateCalculatorClass() {
    std::cout << "=== Calculator Class Template with Specialization ===\n\n";
    
    std::cout << "1. Numeric calculations:\n";
    std::cout << "Calculator<int>::add(10, 5) = " << Calculator<int>::add(10, 5) << std::endl;
    std::cout << "Calculator<double>::multiply(3.14, 2.0) = " << Calculator<double>::multiply(3.14, 2.0) << std::endl;
    std::cout << "Calculator<float>::divide(10.0f, 3.0f) = " << Calculator<float>::divide(10.0f, 3.0f) << std::endl;
    std::cout << "\n";
    
    std::cout << "2. String operations (specialized):\n";
    std::cout << "Calculator<string>::add(\"Hello\", \" World\") = \"" 
              << Calculator<std::string>::add("Hello", " World") << "\"" << std::endl;
    std::cout << "Calculator<string>::subtract(\"Hello World\", \"o\") = \"" 
              << Calculator<std::string>::subtract("Hello World", "o") << "\"" << std::endl;
    std::cout << "Calculator<string>::multiply(\"Hi!\", \"3\") = \"" 
              << Calculator<std::string>::multiply("Hi!", "3") << "\"" << std::endl;
    std::cout << "Calculator<string>::divide(\"Hello\", \"World\") = \"" 
              << Calculator<std::string>::divide("Hello", "World") << "\"" << std::endl;
    std::cout << "\n";
}

void demonstrateTemplateInstantiation() {
    std::cout << "=== Understanding Template Instantiation ===\n\n";
    
    std::cout << "When you declare template class instances:\n";
    std::cout << "Pair<int, double> p1;        -> Compiler generates Pair<int, double> class\n";
    std::cout << "Pair<string, int> p2;        -> Compiler generates Pair<string, int> class\n";
    std::cout << "Array<int, 5> arr1;          -> Compiler generates Array<int, 5> class\n";
    std::cout << "Array<double, 10> arr2;      -> Compiler generates Array<double, 10> class\n\n";
    
    std::cout << "Each instantiation is a complete, separate class with:\n";
    std::cout << "- Type-specific member variables\n";
    std::cout << "- Type-specific member functions\n";
    std::cout << "- Compile-time optimization\n";
    std::cout << "- No runtime overhead\n\n";
    
    std::cout << "This is how STL containers work:\n";
    std::cout << "std::vector<int>, std::vector<string>, std::map<string, int>, etc.\n";
    std::cout << "are all separate, optimized classes generated from templates!\n\n";
}

int main() {
    std::cout << "C++ Class Templates Demonstration\n";
    std::cout << "==================================\n\n";
    
    demonstratePairClass();
    std::cout << std::string(60, '-') << "\n\n";
    
    demonstrateArrayClass();
    std::cout << std::string(60, '-') << "\n\n";
    
    demonstrateStackClass();
    std::cout << std::string(60, '-') << "\n\n";
    
    demonstrateCalculatorClass();
    std::cout << std::string(60, '-') << "\n\n";
    
    demonstrateTemplateInstantiation();
    
    return 0;
}

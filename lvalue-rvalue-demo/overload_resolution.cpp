#include <iostream>
#include <string>
#include <utility>

// Simple class for demonstration
class MyClass {
private:
    std::string data;
    int id;
    static int counter;

public:
    // Constructor
    MyClass(const std::string& str) : data(str), id(++counter) {
        std::cout << "Constructor: MyClass(" << data << ") created with id=" << id << "\n";
    }
    
    // Copy constructor
    MyClass(const MyClass& other) : data(other.data), id(++counter) {
        std::cout << "Copy Constructor: MyClass copied from id=" << other.id 
                  << " to new id=" << id << " (data: " << data << ")\n";
    }
    
    // Move constructor
    MyClass(MyClass&& other) noexcept : data(std::move(other.data)), id(++counter) {
        std::cout << "Move Constructor: MyClass moved from id=" << other.id 
                  << " to new id=" << id << " (data: " << data << ")\n";
        other.data = "[moved]";
    }
    
    // Destructor
    ~MyClass() {
        std::cout << "Destructor: MyClass id=" << id << " (data: " << data << ") destroyed\n";
    }
    
    const std::string& getData() const { return data; }
    int getId() const { return id; }
};

int MyClass::counter = 0;

// Function overloaded for L-value reference
void processObject(MyClass& obj) {
    std::cout << "*** L-VALUE REFERENCE VERSION ***\n";
    std::cout << "Processing L-value reference to object id=" << obj.getId() 
              << " (data: " << obj.getData() << ")\n";
    std::cout << "This version is called when you pass an existing object.\n\n";
}

// Function overloaded for R-value reference  
void processObject(MyClass&& obj) {
    std::cout << "*** R-VALUE REFERENCE VERSION ***\n";
    std::cout << "Processing R-value reference to object id=" << obj.getId() 
              << " (data: " << obj.getData() << ")\n";
    std::cout << "This version is called when you pass a temporary object.\n";
    std::cout << "We can safely move from this object since it's temporary.\n\n";
}

// Function that accepts const L-value reference (can bind to both l-values and r-values)
void processObjectConst(const MyClass& obj) {
    std::cout << "*** CONST L-VALUE REFERENCE VERSION ***\n";
    std::cout << "Processing const reference to object id=" << obj.getId() 
              << " (data: " << obj.getData() << ")\n";
    std::cout << "This version can accept both l-values and r-values,\n";
    std::cout << "but we cannot modify the object.\n\n";
}

// Template function demonstrating perfect forwarding
template<typename T>
void forwardToProcess(T&& obj) {
    std::cout << "*** PERFECT FORWARDING ***\n";
    std::cout << "Template received parameter, forwarding to appropriate overload...\n";
    processObject(std::forward<T>(obj));
}

void demonstrateOverloadResolution() {
    std::cout << "=== Function Overload Resolution Demo ===\n\n";
    
    // Create an l-value object
    std::cout << "1. Creating an l-value object:\n";
    MyClass obj1("persistent");
    std::cout << "\n";
    
    // Call with l-value - calls l-value reference version
    std::cout << "2. Calling processObject with l-value:\n";
    processObject(obj1);
    
    // Call with r-value (temporary) - calls r-value reference version
    std::cout << "3. Calling processObject with r-value (temporary):\n";
    processObject(MyClass("temporary"));
    
    // Call with std::move (converts l-value to r-value) - calls r-value reference version
    std::cout << "4. Calling processObject with std::move (l-value cast to r-value):\n";
    processObject(std::move(obj1));
    std::cout << "Note: obj1 is now in a moved-from state!\n\n";
    
    std::cout << "5. Creating another object for const reference demo:\n";
    MyClass obj2("for_const_demo");
    std::cout << "\n";
    
    // Demonstrate const reference behavior
    std::cout << "6. Const reference can accept both l-values and r-values:\n";
    std::cout << "   a) With l-value:\n";
    processObjectConst(obj2);
    
    std::cout << "   b) With r-value:\n";
    processObjectConst(MyClass("temp_for_const"));
}

void demonstratePerfectForwarding() {
    std::cout << "=== Perfect Forwarding Demo ===\n\n";
    
    std::cout << "1. Creating object for forwarding demo:\n";
    MyClass obj("forwarding_test");
    std::cout << "\n";
    
    std::cout << "2. Perfect forwarding with l-value:\n";
    forwardToProcess(obj);
    
    std::cout << "3. Perfect forwarding with r-value:\n";
    forwardToProcess(MyClass("temp_forward"));
    
    std::cout << "4. Perfect forwarding with moved l-value:\n";
    forwardToProcess(std::move(obj));
}

void demonstrateValueCategories() {
    std::cout << "=== Value Categories Examples ===\n\n";
    
    MyClass obj("value_category_test");
    
    // Different expressions and their value categories
    std::cout << "Expression analysis:\n";
    std::cout << "- 'obj' is an l-value (has name, persistent)\n";
    std::cout << "- 'MyClass(\"temp\")' is an r-value (temporary)\n";
    std::cout << "- 'std::move(obj)' is an r-value (explicit cast)\n";
    std::cout << "- 'obj.getData()' is an l-value (returns reference)\n\n";
    
    // This would work with our overloads:
    // processObject(obj);                    // l-value version
    // processObject(MyClass("temp"));        // r-value version  
    // processObject(std::move(obj));         // r-value version
}

int main() {
    std::cout << "C++ Function Overload Resolution: L-value vs R-value References\n";
    std::cout << "==============================================================\n\n";
    
    demonstrateOverloadResolution();
    std::cout << "\n" << std::string(60, '-') << "\n\n";
    
    demonstratePerfectForwarding();
    std::cout << "\n" << std::string(60, '-') << "\n\n";
    
    demonstrateValueCategories();
    
    std::cout << "\nProgram ending - watch the destructor calls!\n";
    return 0;
}

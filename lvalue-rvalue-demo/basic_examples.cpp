#include <iostream>
#include <string>

void demonstrateBasicConcepts() {
    std::cout << "=== Basic L-value and R-value Examples ===\n\n";
    
    // L-values - expressions that have a memory location
    int x = 10;           // x is an l-value
    int y = 20;           // y is an l-value
    int* ptr = &x;        // ptr is an l-value, &x gives address of l-value
    
    std::cout << "L-values (have memory addresses):\n";
    std::cout << "x = " << x << " (address: " << &x << ")\n";
    std::cout << "y = " << y << " (address: " << &y << ")\n";
    std::cout << "*ptr = " << *ptr << " (address: " << ptr << ")\n\n";
    
    // These are valid l-value assignments
    x = 30;               // x can appear on left side of assignment
    y = x + 5;            // y can appear on left side
    *ptr = 40;            // dereferenced pointer is l-value
    
    std::cout << "After assignments:\n";
    std::cout << "x = " << x << ", y = " << y << "\n\n";
    
    // R-values - temporary values without persistent memory location
    std::cout << "R-values (temporary expressions):\n";
    
    // These are r-values:
    // - Literals: 42, 3.14, "hello"
    // - Temporary objects: std::string("temp")
    // - Results of expressions: x + y, x * 2
    
    int result1 = x + y;     // (x + y) is an r-value
    int result2 = x * 2;     // (x * 2) is an r-value
    std::string temp = std::string("temporary"); // std::string("temporary") is r-value
    
    std::cout << "result1 = x + y = " << result1 << "\n";
    std::cout << "result2 = x * 2 = " << result2 << "\n";
    std::cout << "temp = " << temp << "\n\n";
    
    // These would be compilation errors (uncommenting will cause errors):
    // 10 = x;              // Error: 10 is r-value, can't assign to it
    // (x + y) = 50;        // Error: (x + y) is r-value
    // &(x + y);            // Error: can't take address of r-value
    
    std::cout << "Note: You cannot assign to r-values or take their address\n";
    std::cout << "because they don't have persistent memory locations.\n\n";
}

void demonstrateArraysAndPointers() {
    std::cout << "=== Arrays and Pointers as L-values ===\n\n";
    
    int arr[5] = {1, 2, 3, 4, 5};
    
    // Array elements are l-values
    arr[0] = 10;             // arr[0] is l-value
    arr[2] = arr[1] + arr[3]; // arr[2] is l-value, (arr[1] + arr[3]) is r-value
    
    std::cout << "Array after modifications: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n\n";
    
    // Pointer arithmetic results
    int* p = arr;
    *(p + 1) = 99;           // *(p + 1) is l-value (dereferenced pointer)
    
    std::cout << "After pointer modification: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n\n";
}

void demonstrateConstAndReferences() {
    std::cout << "=== Const and References ===\n\n";
    
    int x = 42;
    const int cx = 100;
    
    // L-value references
    int& ref_x = x;          // ref_x is l-value reference to l-value x
    const int& cref_x = x;   // const reference can bind to l-value
    const int& cref_temp = 50; // const reference can bind to r-value!
    
    std::cout << "L-value references:\n";
    std::cout << "ref_x = " << ref_x << "\n";
    std::cout << "cref_x = " << cref_x << "\n";
    std::cout << "cref_temp = " << cref_temp << " (bound to r-value 50)\n\n";
    
    ref_x = 200;             // Modifying through reference
    std::cout << "After ref_x = 200: x = " << x << "\n\n";
    
    // cx = 300;             // Error: cx is const l-value
    // int& ref_const = cx;  // Error: can't bind non-const ref to const l-value
    // int& ref_rval = 42;   // Error: can't bind non-const ref to r-value
    
    std::cout << "Important: const l-value references can bind to r-values,\n";
    std::cout << "extending the lifetime of the temporary.\n\n";
}

int main() {
    std::cout << "C++ L-values and R-values Demonstration\n";
    std::cout << "======================================\n\n";
    
    demonstrateBasicConcepts();
    demonstrateArraysAndPointers();
    demonstrateConstAndReferences();
    
    return 0;
}

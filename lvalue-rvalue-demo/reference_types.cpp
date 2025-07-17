#include <iostream>
#include <string>
#include <utility>
#include <type_traits>

// Helper function to print type information
template<typename T>
void printTypeInfo(const std::string& varName) {
    std::cout << varName << " type: ";
    
    if constexpr (std::is_lvalue_reference_v<T>) {
        std::cout << "L-value reference (&)";
    } else if constexpr (std::is_rvalue_reference_v<T>) {
        std::cout << "R-value reference (&&)";
    } else {
        std::cout << "Value type";
    }
    
    if constexpr (std::is_const_v<std::remove_reference_t<T>>) {
        std::cout << " [const]";
    }
    
    std::cout << "\n";
}

// Template to demonstrate reference collapsing
template<typename T>
void analyzeReference(T&& param) {
    std::cout << "Template parameter analysis:\n";
    printTypeInfo<T>("T");
    printTypeInfo<T&&>("T&&");
    
    // Check what we actually received
    if constexpr (std::is_lvalue_reference_v<T>) {
        std::cout << "Received: L-value (T is l-value reference)\n";
    } else {
        std::cout << "Received: R-value (T is value type)\n";
    }
    std::cout << "\n";
}

void demonstrateBasicReferences() {
    std::cout << "=== Basic Reference Types ===\n\n";
    
    int x = 42;
    const int cx = 100;
    
    // L-value references
    std::cout << "1. L-value references:\n";
    int& lref = x;           // L-value reference to l-value
    const int& clref1 = x;   // Const l-value reference to l-value
    const int& clref2 = 50;  // Const l-value reference to r-value (lifetime extension!)
    
    std::cout << "   int& lref = x;           // OK: l-value ref to l-value\n";
    std::cout << "   const int& clref1 = x;   // OK: const l-value ref to l-value\n";
    std::cout << "   const int& clref2 = 50;  // OK: const l-value ref to r-value\n";
    std::cout << "   Values: lref=" << lref << ", clref1=" << clref1 << ", clref2=" << clref2 << "\n\n";
    
    // R-value references (C++11)
    std::cout << "2. R-value references:\n";
    int&& rref1 = 75;        // R-value reference to r-value
    int&& rref2 = x + 10;    // R-value reference to r-value expression
    int&& rref3 = std::move(x); // R-value reference to moved l-value
    
    std::cout << "   int&& rref1 = 75;        // OK: r-value ref to r-value literal\n";
    std::cout << "   int&& rref2 = x + 10;    // OK: r-value ref to r-value expression\n";
    std::cout << "   int&& rref3 = std::move(x); // OK: r-value ref to moved l-value\n";
    std::cout << "   Values: rref1=" << rref1 << ", rref2=" << rref2 << ", rref3=" << rref3 << "\n\n";
    
    // Important: once bound, r-value references are l-values!
    std::cout << "3. Important: R-value references become L-values when named:\n";
    rref1 = 99;  // rref1 is now an l-value expression!
    std::cout << "   rref1 = 99;  // rref1 itself is an l-value expression\n";
    std::cout << "   rref1 = " << rref1 << "\n\n";
    
    // These would be compilation errors:
    // int& bad1 = 42;          // Error: can't bind non-const l-value ref to r-value
    // int& bad2 = x + 10;      // Error: can't bind non-const l-value ref to r-value
    // int&& bad3 = x;          // Error: can't bind r-value ref to l-value
    // int&& bad4 = lref;       // Error: can't bind r-value ref to l-value
}

void demonstrateReferenceCollapsing() {
    std::cout << "=== Reference Collapsing Rules ===\n\n";
    
    std::cout << "Reference collapsing rules in templates:\n";
    std::cout << "T&  + &  = T&   (l-value ref + l-value ref = l-value ref)\n";
    std::cout << "T&  + && = T&   (l-value ref + r-value ref = l-value ref)\n";
    std::cout << "T&& + &  = T&   (r-value ref + l-value ref = l-value ref)\n";
    std::cout << "T&& + && = T&&  (r-value ref + r-value ref = r-value ref)\n\n";
    
    int x = 10;
    
    std::cout << "Testing with l-value:\n";
    analyzeReference(x);  // T deduced as int&, T&& becomes int& && -> int&
    
    std::cout << "Testing with r-value:\n";
    analyzeReference(20); // T deduced as int, T&& becomes int&&
    
    std::cout << "Testing with moved l-value:\n";
    analyzeReference(std::move(x)); // T deduced as int, T&& becomes int&&
}

// Functions to demonstrate overload resolution
void overloadTest(int& x) {
    std::cout << "Called: overloadTest(int&) - L-value reference\n";
}

void overloadTest(const int& x) {
    std::cout << "Called: overloadTest(const int&) - Const L-value reference\n";
}

void overloadTest(int&& x) {
    std::cout << "Called: overloadTest(int&&) - R-value reference\n";
}

void demonstrateOverloadResolution() {
    std::cout << "=== Overload Resolution with References ===\n\n";
    
    int x = 42;
    const int cx = 100;
    
    std::cout << "Testing overload resolution:\n";
    
    std::cout << "1. overloadTest(x):           ";
    overloadTest(x);  // Calls int& version
    
    std::cout << "2. overloadTest(cx):          ";
    overloadTest(cx); // Calls const int& version (only viable option)
    
    std::cout << "3. overloadTest(50):          ";
    overloadTest(50); // Calls int&& version
    
    std::cout << "4. overloadTest(std::move(x)): ";
    overloadTest(std::move(x)); // Calls int&& version
    
    std::cout << "\n";
}

// Perfect forwarding example
template<typename T>
void perfectForward(T&& arg) {
    std::cout << "Perfect forwarding: ";
    overloadTest(std::forward<T>(arg));
}

void demonstratePerfectForwarding() {
    std::cout << "=== Perfect Forwarding ===\n\n";
    
    int x = 42;
    const int cx = 100;
    
    std::cout << "Perfect forwarding preserves value categories:\n";
    
    std::cout << "1. perfectForward(x):           ";
    perfectForward(x);  // Forwards as l-value
    
    std::cout << "2. perfectForward(cx):          ";
    perfectForward(cx); // Forwards as const l-value
    
    std::cout << "3. perfectForward(50):          ";
    perfectForward(50); // Forwards as r-value
    
    std::cout << "4. perfectForward(std::move(x)): ";
    perfectForward(std::move(x)); // Forwards as r-value
    
    std::cout << "\n";
}

// Demonstration of reference lifetime issues
void demonstrateLifetimeExtension() {
    std::cout << "=== Reference Lifetime Extension ===\n\n";
    
    std::cout << "1. Const l-value references extend temporary lifetime:\n";
    {
        const std::string& ref = std::string("temporary"); // Lifetime extended!
        std::cout << "   Temporary string bound to const ref: " << ref << "\n";
        std::cout << "   Temporary lives until end of this scope.\n";
    } // Temporary destroyed here
    std::cout << "   Temporary destroyed when scope ended.\n\n";
    
    std::cout << "2. R-value references also extend lifetime:\n";
    {
        std::string&& rref = std::string("another_temp"); // Lifetime extended!
        std::cout << "   Temporary string bound to r-value ref: " << rref << "\n";
        rref += " modified"; // Can modify through r-value reference
        std::cout << "   Modified: " << rref << "\n";
    } // Temporary destroyed here
    std::cout << "   Temporary destroyed when scope ended.\n\n";
    
    // This would be dangerous:
    // const std::string& dangling = std::string("temp").substr(0, 2);
    // The temporary string is destroyed, but substr returns a new temporary
    // that's not bound to the reference!
}

int main() {
    std::cout << "C++ Reference Types: Comprehensive Analysis\n";
    std::cout << "==========================================\n\n";
    
    demonstrateBasicReferences();
    std::cout << std::string(60, '-') << "\n\n";
    
    demonstrateReferenceCollapsing();
    std::cout << std::string(60, '-') << "\n\n";
    
    demonstrateOverloadResolution();
    std::cout << std::string(60, '-') << "\n\n";
    
    demonstratePerfectForwarding();
    std::cout << std::string(60, '-') << "\n\n";
    
    demonstrateLifetimeExtension();
    
    return 0;
}

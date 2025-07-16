// unique_ptr_demo.cpp
// Demonstrates modern C++ memory management with std::unique_ptr and RAII
#include <iostream>
#include <memory>

class Resource {
public:
    Resource(int id) : id_(id) {
        std::cout << "Resource " << id_ << " acquired.\n";
    }
    ~Resource() {
        std::cout << "Resource " << id_ << " released.\n";
    }
    void doWork() {
        std::cout << "Resource " << id_ << " is working.\n";
    }
private:
    int id_;
};

void rawPointerLeakExample() {
    std::cout << "\n[Raw Pointer Example: Memory Leak]\n";
    Resource* res = new Resource(1);
    res->doWork();
    delete res;
    // Forgot to delete res! Memory leak occurs here.
}

void uniquePtrBasicExample() {
    std::cout << "\n[unique_ptr Example: Automatic Cleanup]\n";
    std::unique_ptr<Resource> res = std::make_unique<Resource>(2);
    res->doWork();
    // No need to delete. Resource is released automatically.
}

void uniquePtrOwnershipTransfer() {
    std::cout << "\n[unique_ptr Ownership Transfer]\n";
    std::unique_ptr<Resource> res1 = std::make_unique<Resource>(3);
    res1->doWork();
    std::unique_ptr<Resource> res2 = std::move(res1); // Ownership transferred
    if (!res1) std::cout << "res1 is now nullptr after move.\n";
    res2->doWork();
}

int main() {
    rawPointerLeakExample();
    uniquePtrBasicExample();
    uniquePtrOwnershipTransfer();
    return 0;
}

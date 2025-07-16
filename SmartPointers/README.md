
# SmartPointers: Modern C++ Memory Management

This section demonstrates how to use modern C++ smart pointers (`std::unique_ptr`, `std::shared_ptr`, `std::weak_ptr`) and the RAII idiom to manage resources safely and efficiently.

---

## Key Concepts

### RAII (Resource Acquisition Is Initialization)
RAII is a C++ technique where resource management (memory, files, etc.) is tied to object lifetime. When an object is created, it acquires a resource; when it goes out of scope, its destructor releases the resource. This ensures no leaks and automatic cleanup.

---

## 1. `std::unique_ptr`

- **Exclusive ownership**: Only one `unique_ptr` can own a resource at a time.
- **Automatic cleanup**: The resource is deleted when the `unique_ptr` goes out of scope.
- **No copy, only move**: Ownership can be transferred using `std::move`.

**Example:**
```cpp
#include <memory>
#include <iostream>

struct Resource {
    Resource() { std::cout << "Resource acquired\n"; }
    ~Resource() { std::cout << "Resource released\n"; }
    void doWork() { std::cout << "Working...\n"; }
};

int main() {
    std::unique_ptr<Resource> ptr = std::make_unique<Resource>();
    ptr->doWork();
    // Ownership transfer
    std::unique_ptr<Resource> ptr2 = std::move(ptr);
    if (!ptr) std::cout << "ptr is now nullptr after move.\n";
    ptr2->doWork();
} // Resource released automatically
```

---

## 2. `std::shared_ptr`

- **Shared ownership**: Multiple `shared_ptr`s can own the same resource.
- **Reference counting**: The resource is deleted when the last `shared_ptr` is destroyed.

**Example:**
```cpp
#include <memory>
#include <iostream>

struct Resource {
    Resource() { std::cout << "Resource acquired\n"; }
    ~Resource() { std::cout << "Resource released\n"; }
};

int main() {
    std::shared_ptr<Resource> p1 = std::make_shared<Resource>();
    {
        std::shared_ptr<Resource> p2 = p1;
        std::cout << "Shared count: " << p1.use_count() << "\n";
    } // p2 goes out of scope
    std::cout << "Shared count: " << p1.use_count() << "\n";
} // Resource released when last shared_ptr is destroyed
```

---

## 3. `std::weak_ptr` and Breaking Cycles

- **Non-owning observer**: `weak_ptr` does not affect the reference count.
- **Prevents memory leaks in cyclic graphs**: Use `weak_ptr` for back-pointers or parent pointers.

**Circular Dependency Example:**
```cpp
#include <memory>
#include <iostream>

struct B; // Forward declaration
struct A {
    std::shared_ptr<B> b_ptr;
    ~A() { std::cout << "A destroyed\n"; }
};
struct B {
    std::weak_ptr<A> a_ptr; // Use weak_ptr to break the cycle
    ~B() { std::cout << "B destroyed\n"; }
};

int main() {
    std::shared_ptr<A> a = std::make_shared<A>();
    std::shared_ptr<B> b = std::make_shared<B>();
    a->b_ptr = b;
    b->a_ptr = a;
} // Both destructors are called, no memory leak
```

---

## Summary Table

| Smart Pointer      | Ownership         | Copyable | Reference Count | Use Case                        |
|--------------------|-------------------|----------|----------------|---------------------------------|
| `std::unique_ptr`  | Exclusive         | No       | No             | Sole ownership, fast cleanup    |
| `std::shared_ptr`  | Shared            | Yes      | Yes            | Shared ownership, graphs        |
| `std::weak_ptr`    | Non-owning/Weak   | Yes      | No             | Break cycles, observe resource  |

---

See the code files in this folder for hands-on examples and more details.

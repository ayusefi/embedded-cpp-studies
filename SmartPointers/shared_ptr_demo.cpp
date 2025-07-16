#include <memory>
#include <iostream>

struct B; // Forward declaration

struct A {
    std::shared_ptr<B> b_ptr;
    ~A() {
        std::cout << "A destroyed\n";
    }
   
};

struct B {
    std::weak_ptr<A> a_ptr;
    ~B() {
        std::cout << "B destroyed\n";
    }
};

int main() {
    std::shared_ptr<A> a_ptr = std::make_shared<A>();
    std::shared_ptr<B> b_ptr = std::make_shared<B>();
    a_ptr->b_ptr = b_ptr;
    b_ptr->a_ptr = a_ptr;
}
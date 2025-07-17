#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <memory>

// Resource-heavy class to demonstrate move semantics benefits
class ResourceHolder {
private:
    std::vector<int> data;
    std::string name;
    std::unique_ptr<int[]> buffer;
    size_t buffer_size;
    
public:
    // Constructor
    ResourceHolder(const std::string& n, size_t size = 1000) 
        : name(n), buffer_size(size) {
        std::cout << "Constructor: Creating " << name << " with " << size << " elements\n";
        
        // Allocate large data structures
        data.reserve(size);
        for (size_t i = 0; i < size; ++i) {
            data.push_back(static_cast<int>(i));
        }
        
        buffer = std::make_unique<int[]>(buffer_size);
        for (size_t i = 0; i < buffer_size; ++i) {
            buffer[i] = static_cast<int>(i * 2);
        }
        
        std::cout << "  -> " << name << " construction complete (expensive!)\n";
    }
    
    // Copy constructor (expensive!)
    ResourceHolder(const ResourceHolder& other) 
        : data(other.data), name(other.name + "_copy"), buffer_size(other.buffer_size) {
        std::cout << "Copy Constructor: Copying " << other.name << " to " << name << "\n";
        std::cout << "  -> Copying " << buffer_size << " elements (expensive!)\n";
        
        // Deep copy of dynamic memory
        buffer = std::make_unique<int[]>(buffer_size);
        for (size_t i = 0; i < buffer_size; ++i) {
            buffer[i] = other.buffer[i];
        }
        
        std::cout << "  -> Copy complete\n";
    }
    
    // Move constructor (efficient!)
    ResourceHolder(ResourceHolder&& other) noexcept 
        : data(std::move(other.data)), 
          name(std::move(other.name)), 
          buffer(std::move(other.buffer)),
          buffer_size(other.buffer_size) {
        std::cout << "Move Constructor: Moving resources from " << other.name << "\n";
        std::cout << "  -> No copying, just transferring ownership (efficient!)\n";
        
        // Reset other object to valid but empty state
        other.name = "[moved]";
        other.buffer_size = 0;
        
        std::cout << "  -> Move complete\n";
    }
    
    // Copy assignment operator
    ResourceHolder& operator=(const ResourceHolder& other) {
        if (this != &other) {
            std::cout << "Copy Assignment: " << name << " = " << other.name << "\n";
            std::cout << "  -> Copying " << other.buffer_size << " elements (expensive!)\n";
            
            data = other.data;
            name = other.name + "_assigned";
            buffer_size = other.buffer_size;
            
            buffer = std::make_unique<int[]>(buffer_size);
            for (size_t i = 0; i < buffer_size; ++i) {
                buffer[i] = other.buffer[i];
            }
            
            std::cout << "  -> Copy assignment complete\n";
        }
        return *this;
    }
    
    // Move assignment operator
    ResourceHolder& operator=(ResourceHolder&& other) noexcept {
        if (this != &other) {
            std::cout << "Move Assignment: Moving to " << name << " from " << other.name << "\n";
            std::cout << "  -> No copying, just transferring ownership (efficient!)\n";
            
            data = std::move(other.data);
            name = std::move(other.name);
            buffer = std::move(other.buffer);
            buffer_size = other.buffer_size;
            
            other.name = "[moved]";
            other.buffer_size = 0;
            
            std::cout << "  -> Move assignment complete\n";
        }
        return *this;
    }
    
    // Destructor
    ~ResourceHolder() {
        std::cout << "Destructor: Destroying " << name 
                  << " (size: " << buffer_size << ")\n";
    }
    
    // Getters
    const std::string& getName() const { return name; }
    size_t getSize() const { return data.size(); }
    size_t getBufferSize() const { return buffer_size; }
    
    void printInfo() const {
        std::cout << "  Resource info: " << name 
                  << ", vector size: " << data.size()
                  << ", buffer size: " << buffer_size << "\n";
    }
};

// Function that returns by value (potential move)
ResourceHolder createResource(const std::string& name) {
    std::cout << "Creating resource in function...\n";
    return ResourceHolder(name + "_created", 500);  // RVO or move
}

// Function demonstrating different parameter passing
void processResource(ResourceHolder obj) {  // Pass by value
    std::cout << "Processing resource by value:\n";
    obj.printInfo();
    std::cout << "Function parameter will be destroyed when function ends.\n";
}

void demonstrateMoveSemantics() {
    std::cout << "=== Move Semantics Demonstration ===\n\n";
    
    // 1. Create original object
    std::cout << "1. Creating original resource:\n";
    ResourceHolder original("original", 800);
    original.printInfo();
    std::cout << "\n";
    
    // 2. Copy construction (expensive)
    std::cout << "2. Copy construction (expensive operation):\n";
    ResourceHolder copied = original;  // Copy constructor called
    copied.printInfo();
    std::cout << "\n";
    
    // 3. Move construction (efficient)
    std::cout << "3. Move construction (efficient operation):\n";
    ResourceHolder moved = std::move(original);  // Move constructor called
    moved.printInfo();
    std::cout << "Original after move:\n";
    original.printInfo();  // Should show moved state
    std::cout << "\n";
    
    // 4. Function return (RVO or move)
    std::cout << "4. Function return (Return Value Optimization or move):\n";
    ResourceHolder from_function = createResource("function_return");
    from_function.printInfo();
    std::cout << "\n";
    
    // 5. Assignment operations
    std::cout << "5. Assignment operations:\n";
    ResourceHolder target("target", 300);
    target.printInfo();
    std::cout << "\n";
    
    std::cout << "   a) Copy assignment:\n";
    target = copied;  // Copy assignment
    target.printInfo();
    std::cout << "\n";
    
    std::cout << "   b) Move assignment:\n";
    target = std::move(moved);  // Move assignment
    target.printInfo();
    std::cout << "Moved object after assignment:\n";
    moved.printInfo();
    std::cout << "\n";
}

void demonstrateContainerOperations() {
    std::cout << "=== Container Operations with Move Semantics ===\n\n";
    
    std::vector<ResourceHolder> container;
    container.reserve(3);  // Avoid reallocations
    
    std::cout << "1. Adding elements to vector:\n";
    
    // Push back with copy
    std::cout << "   a) Push back with copy:\n";
    ResourceHolder obj1("obj1", 200);
    container.push_back(obj1);  // Copy
    std::cout << "\n";
    
    // Push back with move
    std::cout << "   b) Push back with move:\n";
    ResourceHolder obj2("obj2", 200);
    container.push_back(std::move(obj2));  // Move
    std::cout << "obj2 after move: " << obj2.getName() << "\n\n";
    
    // Emplace back (construct in place)
    std::cout << "   c) Emplace back (construct in place):\n";
    container.emplace_back("obj3_emplaced", 200);  // Direct construction
    std::cout << "\n";
    
    std::cout << "2. Container contents:\n";
    for (size_t i = 0; i < container.size(); ++i) {
        std::cout << "   [" << i << "] ";
        container[i].printInfo();
    }
    std::cout << "\n";
}

void demonstratePassingToFunctions() {
    std::cout << "=== Passing Objects to Functions ===\n\n";
    
    ResourceHolder obj("function_param", 300);
    
    std::cout << "1. Pass by value with copy:\n";
    processResource(obj);  // Copy constructor called
    std::cout << "Original object after function call:\n";
    obj.printInfo();
    std::cout << "\n";
    
    std::cout << "2. Pass by value with move:\n";
    ResourceHolder obj2("function_param2", 300);
    processResource(std::move(obj2));  // Move constructor called
    std::cout << "Original object after move:\n";
    obj2.printInfo();
    std::cout << "\n";
}

int main() {
    std::cout << "C++ Move Semantics and Resource Management\n";
    std::cout << "=========================================\n\n";
    
    demonstrateMoveSemantics();
    std::cout << std::string(60, '-') << "\n\n";
    
    demonstrateContainerOperations();
    std::cout << std::string(60, '-') << "\n\n";
    
    demonstratePassingToFunctions();
    
    std::cout << "\nProgram ending - watch the destructor cascade!\n";
    return 0;
}

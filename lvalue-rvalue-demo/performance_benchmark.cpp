#include <iostream>
#include <vector>
#include <chrono>
#include <memory>
#include <string>
#include <iomanip>
#include <random>

// Expensive resource class for benchmarking
class ExpensiveResource {
private:
    std::vector<double> data;
    std::unique_ptr<int[]> buffer;
    size_t buffer_size;
    std::string name;
    
    // Helper to generate random data
    void fillWithRandomData() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dis(0.0, 1000.0);
        
        for (size_t i = 0; i < data.size(); ++i) {
            data[i] = dis(gen);
        }
        
        for (size_t i = 0; i < buffer_size; ++i) {
            buffer[i] = static_cast<int>(dis(gen));
        }
    }
    
public:
    // Constructor - creates expensive resource
    ExpensiveResource(const std::string& resource_name, size_t size = 1000000) 
        : name(resource_name), buffer_size(size) {
        
        // Allocate large vector
        data.resize(size);
        
        // Allocate large buffer
        buffer = std::make_unique<int[]>(buffer_size);
        
        // Fill with data (expensive operation)
        fillWithRandomData();
    }
    
    // Copy constructor - EXPENSIVE deep copy
    ExpensiveResource(const ExpensiveResource& other) 
        : data(other.data),  // Vector copy constructor - deep copy!
          buffer_size(other.buffer_size),
          name(other.name + "_copy") {
        
        // Manual deep copy of buffer
        buffer = std::make_unique<int[]>(buffer_size);
        for (size_t i = 0; i < buffer_size; ++i) {
            buffer[i] = other.buffer[i];
        }
    }
    
    // Move constructor - EFFICIENT resource transfer
    ExpensiveResource(ExpensiveResource&& other) noexcept 
        : data(std::move(other.data)),        // Vector move - steals internal pointer!
          buffer(std::move(other.buffer)),    // unique_ptr move - transfers ownership!
          buffer_size(other.buffer_size),
          name(std::move(other.name)) {
        
        // Reset other to valid empty state
        other.buffer_size = 0;
    }
    
    // Copy assignment - EXPENSIVE
    ExpensiveResource& operator=(const ExpensiveResource& other) {
        if (this != &other) {
            data = other.data;  // Deep copy
            buffer_size = other.buffer_size;
            name = other.name + "_assigned";
            
            buffer = std::make_unique<int[]>(buffer_size);
            for (size_t i = 0; i < buffer_size; ++i) {
                buffer[i] = other.buffer[i];
            }
        }
        return *this;
    }
    
    // Move assignment - EFFICIENT
    ExpensiveResource& operator=(ExpensiveResource&& other) noexcept {
        if (this != &other) {
            data = std::move(other.data);
            buffer = std::move(other.buffer);
            buffer_size = other.buffer_size;
            name = std::move(other.name);
            
            other.buffer_size = 0;
        }
        return *this;
    }
    
    // Getters for verification
    size_t getDataSize() const { return data.size(); }
    size_t getBufferSize() const { return buffer_size; }
    const std::string& getName() const { return name; }
    double getDataSum() const {
        double sum = 0.0;
        for (const auto& val : data) {
            sum += val;
        }
        return sum;
    }
};

// Benchmark helper class
class Benchmark {
private:
    std::chrono::high_resolution_clock::time_point start_time;
    
public:
    void start() {
        start_time = std::chrono::high_resolution_clock::now();
    }
    
    double getElapsedMs() const {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        return duration.count() / 1000.0;  // Convert to milliseconds
    }
};

// Function to create and return by value (for RVO testing)
ExpensiveResource createResource(const std::string& name, size_t size) {
    return ExpensiveResource(name, size);
}

void benchmarkCopyVsMove() {
    std::cout << "=== Copy vs Move Performance Benchmark ===\n\n";
    
    const size_t RESOURCE_SIZE = 2000000;  // 2 million elements
    const int ITERATIONS = 50;
    
    std::cout << "Resource size: " << RESOURCE_SIZE << " elements\n";
    std::cout << "Iterations: " << ITERATIONS << "\n\n";
    
    // Create original resource
    std::cout << "Creating original resource...\n";
    ExpensiveResource original("benchmark_resource", RESOURCE_SIZE);
    std::cout << "Original resource created with " << original.getDataSize() << " elements\n\n";
    
    Benchmark timer;
    double copy_total_time = 0.0;
    double move_total_time = 0.0;
    
    // Benchmark COPY operations
    std::cout << "Benchmarking COPY operations...\n";
    for (int i = 0; i < ITERATIONS; ++i) {
        timer.start();
        ExpensiveResource copied = original;  // Copy constructor
        copy_total_time += timer.getElapsedMs();
        
        // Verify the copy worked
        if (copied.getDataSize() != original.getDataSize()) {
            std::cerr << "Copy verification failed!\n";
        }
    }
    
    // Benchmark MOVE operations
    std::cout << "Benchmarking MOVE operations...\n";
    for (int i = 0; i < ITERATIONS; ++i) {
        // Create a new resource for each move test
        ExpensiveResource temp("temp_" + std::to_string(i), RESOURCE_SIZE);
        
        timer.start();
        ExpensiveResource moved = std::move(temp);  // Move constructor
        move_total_time += timer.getElapsedMs();
        
        // Verify the move worked
        if (moved.getDataSize() != RESOURCE_SIZE) {
            std::cerr << "Move verification failed!\n";
        }
    }
    
    // Calculate and display results
    double avg_copy_time = copy_total_time / ITERATIONS;
    double avg_move_time = move_total_time / ITERATIONS;
    double speedup = avg_copy_time / avg_move_time;
    
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "\n=== BENCHMARK RESULTS ===\n";
    std::cout << "Average Copy Time:  " << avg_copy_time << " ms\n";
    std::cout << "Average Move Time:  " << avg_move_time << " ms\n";
    std::cout << "Speedup Factor:     " << speedup << "x faster\n";
    std::cout << "Time Saved:         " << (avg_copy_time - avg_move_time) << " ms per operation\n\n";
    
    if (speedup > 10) {
        std::cout << "🚀 Move semantics provide SIGNIFICANT performance improvement!\n";
    } else if (speedup > 2) {
        std::cout << "✅ Move semantics provide notable performance improvement.\n";
    } else {
        std::cout << "⚠️  Move semantics provide modest improvement (compiler optimizations may be active).\n";
    }
}

void benchmarkContainerOperations() {
    std::cout << "\n=== Container Operations Benchmark ===\n\n";
    
    const size_t RESOURCE_SIZE = 500000;
    const int ITERATIONS = 20;
    
    std::vector<ExpensiveResource> container;
    container.reserve(ITERATIONS);  // Avoid reallocation overhead
    
    Benchmark timer;
    double push_copy_time = 0.0;
    double push_move_time = 0.0;
    double emplace_time = 0.0;
    
    // Benchmark push_back with copy
    std::cout << "Benchmarking push_back with COPY...\n";
    for (int i = 0; i < ITERATIONS; ++i) {
        ExpensiveResource resource("copy_test_" + std::to_string(i), RESOURCE_SIZE);
        
        timer.start();
        container.push_back(resource);  // Copy into container
        push_copy_time += timer.getElapsedMs();
    }
    container.clear();
    
    // Benchmark push_back with move
    std::cout << "Benchmarking push_back with MOVE...\n";
    for (int i = 0; i < ITERATIONS; ++i) {
        ExpensiveResource resource("move_test_" + std::to_string(i), RESOURCE_SIZE);
        
        timer.start();
        container.push_back(std::move(resource));  // Move into container
        push_move_time += timer.getElapsedMs();
    }
    container.clear();
    
    // Benchmark emplace_back (construct in place)
    std::cout << "Benchmarking emplace_back (construct in place)...\n";
    for (int i = 0; i < ITERATIONS; ++i) {
        timer.start();
        container.emplace_back("emplace_test_" + std::to_string(i), RESOURCE_SIZE);
        emplace_time += timer.getElapsedMs();
    }
    
    // Calculate results
    double avg_push_copy = push_copy_time / ITERATIONS;
    double avg_push_move = push_move_time / ITERATIONS;
    double avg_emplace = emplace_time / ITERATIONS;
    
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "\n=== CONTAINER BENCHMARK RESULTS ===\n";
    std::cout << "push_back(copy):  " << avg_push_copy << " ms\n";
    std::cout << "push_back(move):  " << avg_push_move << " ms\n";
    std::cout << "emplace_back:     " << avg_emplace << " ms\n\n";
    
    std::cout << "Copy vs Move speedup:      " << (avg_push_copy / avg_push_move) << "x\n";
    std::cout << "Copy vs Emplace speedup:   " << (avg_push_copy / avg_emplace) << "x\n";
    std::cout << "Move vs Emplace speedup:   " << (avg_push_move / avg_emplace) << "x\n\n";
}

void benchmarkAssignmentOperations() {
    std::cout << "=== Assignment Operations Benchmark ===\n\n";
    
    const size_t RESOURCE_SIZE = 1000000;
    const int ITERATIONS = 30;
    
    Benchmark timer;
    double copy_assign_time = 0.0;
    double move_assign_time = 0.0;
    
    // Create target object
    ExpensiveResource target("target", RESOURCE_SIZE);
    
    // Benchmark copy assignment
    std::cout << "Benchmarking COPY assignment...\n";
    for (int i = 0; i < ITERATIONS; ++i) {
        ExpensiveResource source("source_" + std::to_string(i), RESOURCE_SIZE);
        
        timer.start();
        target = source;  // Copy assignment
        copy_assign_time += timer.getElapsedMs();
    }
    
    // Benchmark move assignment
    std::cout << "Benchmarking MOVE assignment...\n";
    for (int i = 0; i < ITERATIONS; ++i) {
        ExpensiveResource source("source_" + std::to_string(i), RESOURCE_SIZE);
        
        timer.start();
        target = std::move(source);  // Move assignment
        move_assign_time += timer.getElapsedMs();
    }
    
    // Calculate results
    double avg_copy_assign = copy_assign_time / ITERATIONS;
    double avg_move_assign = move_assign_time / ITERATIONS;
    
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "\n=== ASSIGNMENT BENCHMARK RESULTS ===\n";
    std::cout << "Copy Assignment:  " << avg_copy_assign << " ms\n";
    std::cout << "Move Assignment:  " << avg_move_assign << " ms\n";
    std::cout << "Speedup Factor:   " << (avg_copy_assign / avg_move_assign) << "x faster\n\n";
}

void memoryUsageAnalysis() {
    std::cout << "=== Memory Usage Analysis ===\n\n";
    
    const size_t ELEMENT_COUNT = 1000000;
    
    // Calculate theoretical memory usage
    size_t vector_memory = ELEMENT_COUNT * sizeof(double);
    size_t buffer_memory = ELEMENT_COUNT * sizeof(int);
    size_t total_per_object = vector_memory + buffer_memory;
    
    std::cout << "Per object memory usage:\n";
    std::cout << "Vector (double): " << (vector_memory / 1024 / 1024) << " MB\n";
    std::cout << "Buffer (int):    " << (buffer_memory / 1024 / 1024) << " MB\n";
    std::cout << "Total per object: " << (total_per_object / 1024 / 1024) << " MB\n\n";
    
    std::cout << "Copy vs Move memory implications:\n";
    std::cout << "COPY: Allocates " << (total_per_object / 1024 / 1024) << " MB additional memory\n";
    std::cout << "MOVE: Transfers ownership, 0 MB additional memory\n\n";
    
    std::cout << "For 100 objects:\n";
    std::cout << "Copy approach: " << (100 * total_per_object / 1024 / 1024) << " MB total\n";
    std::cout << "Move approach: " << (total_per_object / 1024 / 1024) << " MB total\n";
    std::cout << "Memory saved: " << (99 * total_per_object / 1024 / 1024) << " MB\n\n";
}

int main() {
    std::cout << "C++ Move Semantics Performance Benchmark\n";
    std::cout << "========================================\n\n";
    
    // Warm up the system
    std::cout << "Warming up system...\n";
    ExpensiveResource warmup("warmup", 100000);
    ExpensiveResource warmup_copy = warmup;
    ExpensiveResource warmup_move = std::move(warmup_copy);
    std::cout << "Warm-up complete.\n\n";
    
    benchmarkCopyVsMove();
    std::cout << std::string(60, '=') << "\n";
    
    benchmarkContainerOperations();
    std::cout << std::string(60, '=') << "\n";
    
    benchmarkAssignmentOperations();
    std::cout << std::string(60, '=') << "\n";
    
    memoryUsageAnalysis();
    
    std::cout << "=== KEY INSIGHTS ===\n";
    std::cout << "1. Move operations are orders of magnitude faster than copies\n";
    std::cout << "2. emplace_back is typically fastest for containers\n";
    std::cout << "3. Move semantics dramatically reduce memory allocations\n";
    std::cout << "4. Critical for embedded systems with limited resources\n\n";
    
    return 0;
}

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <chrono>
#include <atomic>
#include <random>

// Configuration constants
const int NUM_ITEMS = 50;
const int BUFFER_SIZE = 10;

// === VERSION 1: BUGGY (Race Condition) ===
namespace buggy_version {
    std::queue<int> buffer;
    std::atomic<bool> producer_done{false};
    
    void producer() {
        std::cout << "[BUGGY] Producer starting...\n";
        for (int i = 1; i <= NUM_ITEMS; ++i) {
            // Simulate some work
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            
            // RACE CONDITION: No synchronization!
            if (buffer.size() < BUFFER_SIZE) {
                buffer.push(i);
                std::cout << "[BUGGY] Producer added: " << i << " (buffer size: " << buffer.size() << ")\n";
            }
        }
        producer_done = true;
        std::cout << "[BUGGY] Producer finished!\n";
    }
    
    void consumer() {
        std::cout << "[BUGGY] Consumer starting...\n";
        int consumed_count = 0;
        
        while (!producer_done || !buffer.empty()) {
            // RACE CONDITION: No synchronization!
            if (!buffer.empty()) {
                int item = buffer.front();
                buffer.pop();
                consumed_count++;
                std::cout << "[BUGGY] Consumer consumed: " << item << " (total consumed: " << consumed_count << ")\n";
                
                // Simulate processing time
                std::this_thread::sleep_for(std::chrono::milliseconds(15));
            }
        }
        std::cout << "[BUGGY] Consumer finished! Total consumed: " << consumed_count << "\n";
    }
}

// === VERSION 2: MUTEX ONLY (Busy-Waiting) ===
namespace mutex_only_version {
    std::queue<int> buffer;
    std::mutex buffer_mutex;
    std::atomic<bool> producer_done{false};
    
    void producer() {
        std::cout << "[MUTEX] Producer starting...\n";
        for (int i = 1; i <= NUM_ITEMS; ++i) {
            // Simulate some work
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            
            // Wait for space in buffer (busy-waiting!)
            while (true) {
                std::lock_guard<std::mutex> lock(buffer_mutex);
                if (buffer.size() < BUFFER_SIZE) {
                    buffer.push(i);
                    std::cout << "[MUTEX] Producer added: " << i << " (buffer size: " << buffer.size() << ")\n";
                    break;
                }
                // BUSY-WAITING: Lock is released, but we immediately try again
            }
        }
        producer_done = true;
        std::cout << "[MUTEX] Producer finished!\n";
    }
    
    void consumer() {
        std::cout << "[MUTEX] Consumer starting...\n";
        int consumed_count = 0;
        
        while (true) {
            std::lock_guard<std::mutex> lock(buffer_mutex);
            if (!buffer.empty()) {
                int item = buffer.front();
                buffer.pop();
                consumed_count++;
                std::cout << "[MUTEX] Consumer consumed: " << item << " (total consumed: " << consumed_count << ")\n";
                
                // Release lock before processing (good practice)
                // Note: lock_guard automatically releases here
            } else if (producer_done) {
                break;
            }
            // BUSY-WAITING: Keep looping even when buffer is empty
            std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Small delay to reduce CPU usage
        }
        std::cout << "[MUTEX] Consumer finished! Total consumed: " << consumed_count << "\n";
    }
}

// === VERSION 3: EFFICIENT (Condition Variable) ===
namespace efficient_version {
    std::queue<int> buffer;
    std::mutex buffer_mutex;
    std::condition_variable buffer_not_empty;
    std::condition_variable buffer_not_full;
    bool producer_done = false;
    
    void producer() {
        std::cout << "[EFFICIENT] Producer starting...\n";
        for (int i = 1; i <= NUM_ITEMS; ++i) {
            // Simulate some work
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            
            // Wait for space in buffer
            std::unique_lock<std::mutex> lock(buffer_mutex);
            buffer_not_full.wait(lock, [] { return buffer.size() < BUFFER_SIZE; });
            
            // Add item to buffer
            buffer.push(i);
            std::cout << "[EFFICIENT] Producer added: " << i << " (buffer size: " << buffer.size() << ")\n";
            
            // Notify consumer that data is available
            buffer_not_empty.notify_one();
        }
        
        // Signal that producer is done
        {
            std::lock_guard<std::mutex> lock(buffer_mutex);
            producer_done = true;
        }
        buffer_not_empty.notify_all(); // Wake up all waiting consumers
        std::cout << "[EFFICIENT] Producer finished!\n";
    }
    
    void consumer() {
        std::cout << "[EFFICIENT] Consumer starting...\n";
        int consumed_count = 0;
        
        while (true) {
            std::unique_lock<std::mutex> lock(buffer_mutex);
            
            // Wait for data to be available or producer to be done
            buffer_not_empty.wait(lock, [] { return !buffer.empty() || producer_done; });
            
            if (!buffer.empty()) {
                int item = buffer.front();
                buffer.pop();
                consumed_count++;
                std::cout << "[EFFICIENT] Consumer consumed: " << item << " (total consumed: " << consumed_count << ")\n";
                
                // Notify producer that space is available
                buffer_not_full.notify_one();
                
                // Release lock before processing
                lock.unlock();
                
                // Simulate processing time
                std::this_thread::sleep_for(std::chrono::milliseconds(15));
            } else if (producer_done && buffer.empty()) {
                // Producer is done and buffer is empty
                break;
            }
        }
        std::cout << "[EFFICIENT] Consumer finished! Total consumed: " << consumed_count << "\n";
    }
}

// === DEMONSTRATION FUNCTIONS ===
void run_buggy_version() {
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "RUNNING BUGGY VERSION (Race Conditions)\n";
    std::cout << std::string(60, '=') << "\n";
    
    buggy_version::producer_done = false;
    // Clear the buffer
    while (!buggy_version::buffer.empty()) {
        buggy_version::buffer.pop();
    }
    
    std::thread producer_thread(buggy_version::producer);
    std::thread consumer_thread(buggy_version::consumer);
    
    producer_thread.join();
    consumer_thread.join();
    
    std::cout << "Final buffer size: " << buggy_version::buffer.size() << "\n";
}

void run_mutex_only_version() {
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "RUNNING MUTEX-ONLY VERSION (Busy-Waiting)\n";
    std::cout << std::string(60, '=') << "\n";
    
    mutex_only_version::producer_done = false;
    // Clear the buffer
    {
        std::lock_guard<std::mutex> lock(mutex_only_version::buffer_mutex);
        while (!mutex_only_version::buffer.empty()) {
            mutex_only_version::buffer.pop();
        }
    }
    
    std::thread producer_thread(mutex_only_version::producer);
    std::thread consumer_thread(mutex_only_version::consumer);
    
    producer_thread.join();
    consumer_thread.join();
    
    std::cout << "Final buffer size: " << mutex_only_version::buffer.size() << "\n";
}

void run_efficient_version() {
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "RUNNING EFFICIENT VERSION (Condition Variables)\n";
    std::cout << std::string(60, '=') << "\n";
    
    efficient_version::producer_done = false;
    // Clear the buffer
    {
        std::lock_guard<std::mutex> lock(efficient_version::buffer_mutex);
        while (!efficient_version::buffer.empty()) {
            efficient_version::buffer.pop();
        }
    }
    
    std::thread producer_thread(efficient_version::producer);
    std::thread consumer_thread(efficient_version::consumer);
    
    producer_thread.join();
    consumer_thread.join();
    
    std::cout << "Final buffer size: " << efficient_version::buffer.size() << "\n";
}

int main() {
    std::cout << "Producer-Consumer Problem Demonstration\n";
    std::cout << "=======================================\n";
    std::cout << "This program demonstrates three implementations:\n";
    std::cout << "1. Buggy version with race conditions\n";
    std::cout << "2. Mutex-only version with busy-waiting\n";
    std::cout << "3. Efficient version with condition variables\n\n";
    std::cout << "Configuration:\n";
    std::cout << "- Number of items: " << NUM_ITEMS << "\n";
    std::cout << "- Buffer size: " << BUFFER_SIZE << "\n\n";
    
    try {
        // Run all three versions
        run_buggy_version();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        run_mutex_only_version();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        run_efficient_version();
        
        std::cout << "\n" << std::string(60, '=') << "\n";
        std::cout << "DEMONSTRATION COMPLETE\n";
        std::cout << std::string(60, '=') << "\n";
        std::cout << "\nKey Observations:\n";
        std::cout << "1. Buggy version may show inconsistent results due to race conditions\n";
        std::cout << "2. Mutex-only version is correct but inefficient (busy-waiting)\n";
        std::cout << "3. Efficient version is both correct and efficient (condition variables)\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}

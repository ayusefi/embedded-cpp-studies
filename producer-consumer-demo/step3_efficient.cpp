#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>

// STEP 3: The Elegant Solution with Condition Variables
// Efficient synchronization without busy-waiting

std::queue<int> shared_buffer;
std::mutex buffer_mutex;
std::condition_variable buffer_condition;  // 📡 Our signaling mechanism
bool producer_done = false;

void producer() {
    std::cout << "🏭 Producer: Starting work...\n";
    
    for (int i = 1; i <= 5; ++i) {
        // Simulate making a product
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        // 🔒 CRITICAL SECTION: Add item and notify
        {
            std::lock_guard<std::mutex> lock(buffer_mutex);
            shared_buffer.push(i);
            std::cout << "🏭 Producer: Made item " << i << " (buffer size: " << shared_buffer.size() << ")\n";
        } // 🔓 Lock released here
        
        // 📡 SIGNAL: Wake up sleeping consumers!
        buffer_condition.notify_one();  // "Hey, there's work available!"
    }
    
    // 🔒 Signal that we're done
    {
        std::lock_guard<std::mutex> lock(buffer_mutex);
        producer_done = true;
    }
    
    // 📡 Wake up ALL consumers to let them know we're done
    buffer_condition.notify_all();  // "Wake up everyone, no more work coming!"
    
    std::cout << "🏭 Producer: Finished!\n";
}

void consumer(int id) {
    std::cout << "📦 Consumer " << id << ": Starting work...\n";
    int consumed_count = 0;
    
    while (true) {
        int item = -1;
        bool got_item = false;
        
        // 🔒 WAIT-AND-CHECK PATTERN
        {
            std::unique_lock<std::mutex> lock(buffer_mutex);
            
            // 😴 EFFICIENT WAITING: Sleep until there's work OR producer is done
            buffer_condition.wait(lock, [] {
                return !shared_buffer.empty() || producer_done;
            });
            
            // When we wake up, either there's work or producer is done
            if (!shared_buffer.empty()) {
                item = shared_buffer.front();
                shared_buffer.pop();
                got_item = true;
            } else if (producer_done) {
                // Nothing left to do
                break;
            }
        } // 🔓 Lock released here
        
        if (got_item) {
            consumed_count++;
            std::cout << "📦 Consumer " << id << ": Got item " << item 
                      << " (total consumed: " << consumed_count << ")\n";
            
            // Simulate processing (outside the lock!)
            std::this_thread::sleep_for(std::chrono::milliseconds(150));
        }
    }
    
    std::cout << "📦 Consumer " << id << ": Finished! Total: " << consumed_count << "\n";
}

int main() {
    std::cout << "=== STEP 3: CONDITION VARIABLES (Efficient!) ===\n";
    std::cout << "Now consumers sleep efficiently until work is available!\n\n";
    
    // Reset everything
    while (!shared_buffer.empty()) shared_buffer.pop();
    producer_done = false;
    
    auto start_time = std::chrono::high_resolution_clock::now();
    
    // Start the threads
    std::thread producer_thread(producer);
    std::thread consumer1_thread(consumer, 1);
    std::thread consumer2_thread(consumer, 2);
    
    // Wait for everyone to finish
    producer_thread.join();
    consumer1_thread.join();
    consumer2_thread.join();
    
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    
    std::cout << "\n🎯 Results:\n";
    std::cout << "Final buffer size: " << shared_buffer.size() << "\n";
    std::cout << "Total time: " << duration.count() << " ms\n\n";
    
    std::cout << "✅ What's perfect now:\n";
    std::cout << "- No race conditions\n";
    std::cout << "- No busy-waiting (efficient CPU usage)\n";
    std::cout << "- Consumers sleep until signaled\n";
    std::cout << "- Perfect for embedded systems and ROS 2!\n\n";
    
    std::cout << "🧠 Key Concepts You Just Learned:\n";
    std::cout << "1. 🔒 std::mutex - Protects shared data\n";
    std::cout << "2. 📡 std::condition_variable - Efficient signaling\n";
    std::cout << "3. 😴 .wait() with predicate - Sleep until condition is true\n";
    std::cout << "4. 📢 .notify_one() / .notify_all() - Wake up waiting threads\n";
    std::cout << "5. 🔄 std::unique_lock - Flexible locking for condition variables\n\n";
    
    return 0;
}

#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <chrono>

// STEP 2: Adding Mutex Protection
// This fixes race conditions but introduces inefficiency

std::queue<int> shared_buffer;
std::mutex buffer_mutex;  // 🔒 Our lock for the shared resource
bool producer_done = false;

void producer() {
    std::cout << "🏭 Producer: Starting work...\n";
    
    for (int i = 1; i <= 5; ++i) {
        // Simulate making a product
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        // 🔒 CRITICAL SECTION: Lock before accessing shared data
        {
            std::lock_guard<std::mutex> lock(buffer_mutex);
            shared_buffer.push(i);
            std::cout << "🏭 Producer: Made item " << i << " (buffer size: " << shared_buffer.size() << ")\n";
        } // 🔓 Lock automatically released here
    }
    
    // 🔒 Lock when changing producer_done
    {
        std::lock_guard<std::mutex> lock(buffer_mutex);
        producer_done = true;
    }
    
    std::cout << "🏭 Producer: Finished!\n";
}

void consumer(int id) {
    std::cout << "📦 Consumer " << id << ": Starting work...\n";
    int consumed_count = 0;
    
    while (true) {
        int item = -1;
        bool got_item = false;
        bool should_exit = false;
        
        // 🔒 CRITICAL SECTION: Check and potentially get an item
        {
            std::lock_guard<std::mutex> lock(buffer_mutex);
            
            if (!shared_buffer.empty()) {
                item = shared_buffer.front();
                shared_buffer.pop();
                got_item = true;
            } else if (producer_done) {
                should_exit = true;
            }
        } // 🔓 Lock released here
        
        if (should_exit) {
            break;
        }
        
        if (got_item) {
            consumed_count++;
            std::cout << "📦 Consumer " << id << ": Got item " << item 
                      << " (total consumed: " << consumed_count << ")\n";
            
            // Simulate processing (outside the lock!)
            std::this_thread::sleep_for(std::chrono::milliseconds(150));
        } else {
            // ⚠️ BUSY-WAITING: Keep checking even when there's nothing to do
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    
    std::cout << "📦 Consumer " << id << ": Finished! Total: " << consumed_count << "\n";
}

int main() {
    std::cout << "=== STEP 2: MUTEX PROTECTION (But Inefficient) ===\n";
    std::cout << "This version is correct but wastes CPU with busy-waiting!\n\n";
    
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
    
    std::cout << "✅ What's better:\n";
    std::cout << "- No race conditions!\n";
    std::cout << "- Consistent output\n";
    std::cout << "- Exactly 5 items produced and consumed\n\n";
    
    std::cout << "❌ What's still wrong:\n";
    std::cout << "- Consumers waste CPU constantly checking for work\n";
    std::cout << "- This is called 'busy-waiting' or 'spinning'\n";
    std::cout << "- Inefficient, especially on battery-powered embedded systems!\n\n";
    
    return 0;
}

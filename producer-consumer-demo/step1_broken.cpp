#include <iostream>
#include <thread>
#include <queue>
#include <chrono>

// STEP 1: The Broken Version (No Synchronization)
// This will show you WHY we need synchronization

std::queue<int> shared_buffer;  // Our "conveyor belt"
bool producer_done = false;

void producer() {
    std::cout << "🏭 Producer: Starting work...\n";
    
    for (int i = 1; i <= 5; ++i) {
        // Simulate making a product
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        // ⚠️ DANGEROUS: No protection while accessing shared_buffer!
        shared_buffer.push(i);
        std::cout << "🏭 Producer: Made item " << i << " (buffer size: " << shared_buffer.size() << ")\n";
    }
    
    producer_done = true;
    std::cout << "🏭 Producer: Finished!\n";
}

void consumer(int id) {
    std::cout << "📦 Consumer " << id << ": Starting work...\n";
    int consumed_count = 0;
    
    while (!producer_done || !shared_buffer.empty()) {
        // ⚠️ DANGEROUS: No protection while accessing shared_buffer!
        if (!shared_buffer.empty()) {
            int item = shared_buffer.front();
            shared_buffer.pop();
            consumed_count++;
            
            std::cout << "📦 Consumer " << id << ": Got item " << item 
                      << " (total consumed: " << consumed_count << ")\n";
            
            // Simulate processing the item
            std::this_thread::sleep_for(std::chrono::milliseconds(150));
        }
    }
    
    std::cout << "📦 Consumer " << id << ": Finished! Total: " << consumed_count << "\n";
}

int main() {
    std::cout << "=== STEP 1: THE BROKEN VERSION ===\n";
    std::cout << "Watch carefully - you might see strange behavior!\n\n";
    
    // Reset everything
    while (!shared_buffer.empty()) shared_buffer.pop();
    producer_done = false;
    
    // Start the threads
    std::thread producer_thread(producer);
    std::thread consumer1_thread(consumer, 1);
    std::thread consumer2_thread(consumer, 2);
    
    // Wait for everyone to finish
    producer_thread.join();
    consumer1_thread.join();
    consumer2_thread.join();
    
    std::cout << "\n🎯 Final buffer size: " << shared_buffer.size() << "\n";
    std::cout << "\n❓ Questions for you:\n";
    std::cout << "1. Did you see any weird output mixing?\n";
    std::cout << "2. Were exactly 5 items consumed in total?\n";
    std::cout << "3. Did the buffer size make sense?\n";
    std::cout << "4. Run this several times - do you get the same result?\n\n";
    
    return 0;
}

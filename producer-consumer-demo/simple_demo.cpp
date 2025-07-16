#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>

// Simple Producer-Consumer with condition variables
class ThreadSafeQueue {
private:
    std::queue<int> queue_;
    std::mutex mutex_;
    std::condition_variable condition_;
    bool finished_ = false;

public:
    void push(int item) {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(item);
        condition_.notify_one();
    }
    
    bool pop(int& item) {
        std::unique_lock<std::mutex> lock(mutex_);
        condition_.wait(lock, [this] { return !queue_.empty() || finished_; });
        
        if (!queue_.empty()) {
            item = queue_.front();
            queue_.pop();
            return true;
        }
        return false; // Queue is empty and producer is finished
    }
    
    void finish() {
        std::lock_guard<std::mutex> lock(mutex_);
        finished_ = true;
        condition_.notify_all();
    }
    
    size_t size() const {
        std::lock_guard<std::mutex> lock(const_cast<std::mutex&>(mutex_));
        return queue_.size();
    }
};

void producer(ThreadSafeQueue& queue, int num_items) {
    std::cout << "Producer starting...\n";
    
    for (int i = 1; i <= num_items; ++i) {
        queue.push(i);
        std::cout << "Producer added: " << i << std::endl;
        
        // Simulate work
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    queue.finish();
    std::cout << "Producer finished!\n";
}

void consumer(ThreadSafeQueue& queue, int id) {
    std::cout << "Consumer " << id << " starting...\n";
    int item;
    int count = 0;
    
    while (queue.pop(item)) {
        count++;
        std::cout << "Consumer " << id << " consumed: " << item 
                  << " (total: " << count << ")\n";
        
        // Simulate processing
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
    
    std::cout << "Consumer " << id << " finished! Total consumed: " << count << "\n";
}

int main() {
    const int NUM_ITEMS = 10;
    const int NUM_CONSUMERS = 2;
    
    std::cout << "Simple Producer-Consumer Demo\n";
    std::cout << "Items to produce: " << NUM_ITEMS << "\n";
    std::cout << "Number of consumers: " << NUM_CONSUMERS << "\n\n";
    
    ThreadSafeQueue queue;
    
    // Start producer
    std::thread producer_thread(producer, std::ref(queue), NUM_ITEMS);
    
    // Start consumers
    std::vector<std::thread> consumer_threads;
    for (int i = 0; i < NUM_CONSUMERS; ++i) {
        consumer_threads.emplace_back(consumer, std::ref(queue), i + 1);
    }
    
    // Wait for all threads to complete
    producer_thread.join();
    for (auto& t : consumer_threads) {
        t.join();
    }
    
    std::cout << "\nDemo completed! Final queue size: " << queue.size() << "\n";
    
    return 0;
}

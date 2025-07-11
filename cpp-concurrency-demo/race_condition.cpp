#include <iostream>
#include <thread>
#include <queue>
#include <mutex>

std::queue<int> sharedQueue;
std::mutex queueMutex;

void producer() {
    for (int i = 0; i < 10000; ++i) {
        std::lock_guard<std::mutex> lock(queueMutex);
        sharedQueue.push(i);
        std::cout << "Produced: " << i << std::endl;
    }
}

void consumer() {
    for (int i = 0; i < 10000; ++i) {
        std::lock_guard<std::mutex> lock(queueMutex);
        if (!sharedQueue.empty()) {
            int value = sharedQueue.front();
            sharedQueue.pop();
            std::cout << "Consumed: " << value << std::endl;
        }
    }
}

int main() {
    std::thread t1(producer);
    std::thread t2(consumer);
    t1.join();
    t2.join();
    return 0;
}

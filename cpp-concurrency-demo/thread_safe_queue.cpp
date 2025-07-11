#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

class ThreadSafeQueue {
public:
    void push(int value) {
        std::lock_guard<std::mutex> lock(mtx);
        q.push(value);
        cv.notify_one();
    }

    int pop() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this]{ return !q.empty(); });
        int value = q.front();
        q.pop();
        return value;
    }

private:
    std::queue<int> q;
    std::mutex mtx;
    std::condition_variable cv;
};

ThreadSafeQueue sharedQueue;

void producer() {
    for (int i = 0; i < 10000; ++i) {
        sharedQueue.push(i);
        std::cout << "Produced: " << i << std::endl;
    }
}

void consumer() {
    for (int i = 0; i < 10000; ++i) {
        int value = sharedQueue.pop();
        std::cout << "Consumed: " << value << std::endl;
    }
}

int main() {
    std::thread t1(producer);
    std::thread t2(consumer);
    t1.join();
    t2.join();
    return 0;
}

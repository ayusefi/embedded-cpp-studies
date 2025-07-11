#include <iostream>
#include <thread>
#include <chrono>
#include <future>

// Simulate heavy computation (2-3 seconds)
int heavy_computation() {
    std::cout << "Starting heavy computation...\n";
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << "Heavy computation done!\n";
    return 42;
}

int main() {
    int ticks = 0;
    std::future<int> result_future1;
    std::future<int> result_future2;
    bool started1 = false, started2 = false;
    bool printed1 = false, printed2 = false;
    int N = 5;
    std::vector<std::future<int>>  futures;
    std::vector<bool> printed(N, false);
    
    for (int i = 0; i < N; ++i) {

    }
    while (ticks < 20) {
        std::cout << "." << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        ++ticks;
        if (ticks == 5 && !started1) {
            // Launch first heavy computation asynchronously
            result_future1 = std::async(std::launch::async, heavy_computation);
            started1 = true;
        }
        if (ticks == 10 && !started2) {
            // Launch second heavy computation asynchronously
            result_future2 = std::async(std::launch::async, heavy_computation);
            started2 = true;
        }
        // Periodically check if the first result is ready
        if (started1 && !printed1 && result_future1.valid()) {
            if (result_future1.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                int result = result_future1.get();
                std::cout << "\nResult 1: " << result << "\n";
                printed1 = true;
            }
        }
        // Periodically check if the second result is ready
        if (started2 && !printed2 && result_future2.valid()) {
            if (result_future2.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                int result = result_future2.get();
                std::cout << "\nResult 2: " << result << "\n";
                printed2 = true;
            }
        }
    }
    std::cout << "\nMain loop finished.\n";
    /*
    Manual approach for comparison:

    #include <thread>
    #include <mutex>
    #include <condition_variable>

    std::mutex mtx;
    std::condition_variable cv;
    bool done = false;
    int result = 0;

    void heavy_computation_manual() {
        std::unique_lock<std::mutex> lock(mtx);
        // Simulate heavy work
        std::this_thread::sleep_for(std::chrono::seconds(3));
        result = 42;
        done = true;
        cv.notify_one();
    }

    int main() {
        std::thread worker(heavy_computation_manual);
        int ticks = 0;
        while (ticks < 10) {
            std::cout << "." << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            ++ticks;
            std::unique_lock<std::mutex> lock(mtx);
            if (done) {
                std::cout << "\nResult: " << result << "\n";
                break;
            }
        }
        worker.join();
        std::cout << "\nMain loop finished.\n";
        return 0;
    }

    This manual approach requires explicit thread management, mutexes, and condition variables, making the code longer and harder to maintain. In contrast, std::async and std::future provide a much cleaner and simpler solution for asynchronous tasks.
    */
    return 0;
}

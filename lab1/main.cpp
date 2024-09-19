#include <iostream>
#include <fstream>
#include <thread>
#include <atomic>
// #include <mutex>

class CustomMutualExlusion {
private:
    std::atomic_flag flag = ATOMIC_FLAG_INIT;

public:
    void lock() {
        while (flag.test_and_set(std::memory_order_acquire)) {
            // NULL
        }
    }

    void unlock() {
        flag.clear(std::memory_order_release);
    }
};

std::ofstream logfile("log.txt");
// std::mutex mtx;
CustomMutualExlusion mtx;

void logMessage(const int threadID) {
    for (int i = 0; i < 5; ++i) {
        // std::lock_guard<std::mutex> lock(mtx);
        mtx.lock();
        //-------- Critical section ---------------------------------------------
        logfile << "Thread " << threadID << " logging message " << i << std::endl;
        //-----------------------------------------------------------------------
        mtx.unlock();
    }
}

int main() {
    std::thread t1(logMessage, 1);
    std::thread t2(logMessage, 2);

    t1.join();
    t2.join();

    logfile.close();

    return 0;
}

#include <iostream>
#include <thread>
#include <atomic>
#include <string>

class CustomMutualExclusion {
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

long long sharedTotal = 1;
int logIndex = 0;
std::string sharedLog[100];
CustomMutualExclusion mtx;

void computeSum(const int threadID, const int* data, const int start, const int end, int& logIndex) {
    long long partial = 0;

    std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    // mtx.lock();
    //-------- Critical section ---------------------------------------------
    for (int i = start; i < end; ++i) {
        partial += data[i];
    }

    sharedLog[logIndex++] = "Thread " + std::to_string(threadID) + " current shared value " + std::to_string(partial);
    sharedTotal += partial;
    //-----------------------------------------------------------------------
    // mtx.unlock();
}

// incorrect computation - one thread log is not correct
// Final shared total: 500000500001
// Log of computations:
// Thread 2 current shared value 937512500
// Thread 3 current shared value 1562512500
// Thread 4 current shared value 497187962500

int main() {
    int data[1000000];
    for (int i = 0; i < 1000000; ++i) {
        data[i] = i + 1;
    }

    std::thread t1(computeSum, 1, data, 0, 25000, std::ref(logIndex));
    std::thread t2(computeSum, 2, data, 25000, 50000, std::ref(logIndex));
    std::thread t3(computeSum, 3, data, 50000, 75000, std::ref(logIndex));
    std::thread t4(computeSum, 4, data, 75000, 1000000, std::ref(logIndex));

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    std::cout << "Final shared total: " << sharedTotal << std::endl;
    std::cout << "Log of computations:" << std::endl;

    for(const auto & i : sharedLog) {
        std::cout << i << std::endl;
    }

    return 0;
}
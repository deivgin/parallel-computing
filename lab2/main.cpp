#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <random>

class Barrier {
public:
    explicit Barrier(const int numThreads) : threadCount(numThreads), waiting(0), counter(0) {}

    void waitBarrier() {
        std::unique_lock<std::mutex> lock(mtx);
        int currentGeneration = counter;
        ++counter;

        if (counter == threadCount) {
            // Reset on last thread
            counter = 0;
            ++counter;
            cv.notify_all();
        } else {
            // Wait until all threads reach the barrier
            cv.wait(lock, [this, currentGeneration] {
                return counter != currentGeneration;
            });
        }
    }

private:
    std::mutex mtx;
    std::condition_variable cv;
    int threadCount;
    int waiting;
    int counter;
};

// Simulating image processing: each thread works on a chunk of the image
void processImageChunk(const int threadID, const int chunkID) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(100, 500);

    const int workTime = dis(gen);
    std::this_thread::sleep_for(std::chrono::milliseconds(workTime));

    std::cout << "Thread " << threadID << " processed chunk " << chunkID << " in " << workTime << " ms.\n";
}

// Each thread simulates processing of an image part in several steps
void threadImageProcessing(Barrier& barrier, const int threadID, const int chunks, const int steps) {
    for (int step = 1; step <= steps; ++step) {
        for (int chunkID = 1; chunkID <= chunks; ++chunkID) {
            processImageChunk(threadID, chunkID);  // Process a chunk of the image
        }

        std::cout << "Thread " << threadID << " finished step " << step << ". Waiting for others...\n";
        barrier.waitBarrier();  // Synchronize threads at the end of the step
        std::cout << "Thread " << threadID << " continues to next step after step " << step << ".\n";
    }
}

int main() {
    constexpr int numThreads = 4;

    Barrier barrier(numThreads);

    std::vector<std::thread> threads;

    // Each thread simulates processing parts of the image
    for (int i = 0; i < numThreads; ++i) {
        constexpr int steps = 3;
        constexpr int chunks = 2;

        threads.emplace_back(threadImageProcessing, std::ref(barrier), i, chunks, steps);
    }

    // Join all threads after they finish
    for (auto& th : threads) {
        th.join();
    }

    std::cout << "Image processing completed in all steps.\n";

    return 0;
}

#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <iostream>
#include <iomanip>
#include <omp.h>

void bubbleSort(std::vector<int> data) {
    const size_t n = data.size();

    for (size_t i = 0; i < n - 1; ++i) {
        for (size_t j = 0; j < n - i - 1; ++j) {
            if (data[j] > data[j + 1]) {
                std::swap(data[j], data[j + 1]);
            }
        }
    }
}

void bubbleSortParallel(std::vector<int> data, int numThreads) {
    const size_t n = data.size();
    bool isEvenPhase = true;

    omp_set_num_threads(numThreads);

    for (size_t i = 0; i < n - 1; ++i) {
        #pragma omp parallel for
        for (size_t j = isEvenPhase ? 0 : 1; j < n - 1; j += 2) {
            if (data[j] > data[j + 1]) {
                std::swap(data[j], data[j + 1]);
            }
        }
        isEvenPhase = !isEvenPhase;
    }
}

std::vector<int> generateRandomData(size_t size) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 10000);

    std::vector<int> data(size);
    for (auto& num : data) {
        num = dis(gen);
    }
    return data;
}

template<typename Func>
double measureExecutionTime(Func&& func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(end - start).count();
}

int main() {
    constexpr size_t dataSize = 100000;
    constexpr size_t numThreads = 6;

    auto data1 = generateRandomData(dataSize);
    auto data2 = data1;

    double sequentialTime = measureExecutionTime([&]() {
        bubbleSort(data1);
    });

    double parallelTime = measureExecutionTime([&]() {
        bubbleSortParallel(data2, numThreads);
    });

    std::cout << "Sequential sort time: " << std::fixed << std::setprecision(6)
              << sequentialTime << " seconds\n";
    std::cout << "Parallel sort time (" << numThreads << " threads): "
              << std::fixed << std::setprecision(6) << parallelTime << " seconds\n";
    std::cout << "Speedup: " << std::fixed << std::setprecision(2)
              << (parallelTime > 0 ? sequentialTime / parallelTime : 0) << "x\n";

    return 0;
}
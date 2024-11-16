#include <vector>
#include <iostream>
#include <chrono>
#include <random>
#include <omp.h>

std::vector<int> generateDataset(const size_t size, const int min = 1, const int max = 1000000) {
    std::vector<int> dataset(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);

    for (size_t i = 0; i < size; ++i) {
        dataset[i] = dis(gen);
    }

    return dataset;
}


void showProgramInfo(const size_t size) {
    std::cout << "Dataset size: " << size << std::endl;

    #pragma omp parallel
    {
        #pragma omp single
        {
            std::cout << "Number of threads: " << omp_get_num_threads() << "\n";
        }
    }
}

void parallelBubbleSort(std::vector<int>& arr) {
    const size_t n = arr.size();
    bool swapped = true;

    for (size_t k = 0; swapped && k < n - 1; ++k) {
        swapped = false;

        #pragma omp parallel
        {
            bool localSwapped = false;

            #pragma omp for
            for (size_t i = k % 2; i < n - 1; i += 2) {
                if (arr[i] > arr[i + 1]) {
                    std::swap(arr[i], arr[i + 1]);
                    localSwapped = true;
                }
            }

            #pragma omp atomic
            swapped |= localSwapped;
        }
    }
}

void sequentialBubbleSort(std::vector<int>& arr) {
    const size_t n = arr.size();

    for (size_t i = 0; i < n - 1; ++i) {
        bool swapped = false;
        for (size_t j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) {
            break;
        }
    }
}

void runSequential(std::vector<int> arr) {
    std::cout << "Sorting dataset sequentially...\n";
    const auto start = std::chrono::high_resolution_clock::now();
    sequentialBubbleSort(arr);
    const auto end = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> elapsed = end - start;
    std::cout << "Time taken to sort sequentially: " << elapsed.count() << " seconds\n";
}

void runParallel(std::vector<int> arr) {
    std::cout << "Sorting dataset in parallel...\n";
    const auto start = std::chrono::high_resolution_clock::now();
    parallelBubbleSort(arr);
    const auto end = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> elapsed = end - start;
    std::cout << "Time taken to sort in parallel: " << elapsed.count() << " seconds\n";
}

int main() {
    constexpr size_t size = 100'000;
    showProgramInfo(size);

    const auto sequentialSet = generateDataset(size);
    const auto parallelSet = generateDataset(size);

    runSequential(sequentialSet);
    runParallel(parallelSet);

    return 0;
}
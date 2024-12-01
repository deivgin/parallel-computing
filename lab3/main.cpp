#include <vector>
#include <iostream>
#include <chrono>
#include <random>
#include <omp.h>
#include <fstream>
#include <sstream>

void bubbleSort(std::vector<int>& dataset) {
    const size_t size = dataset.size();

    for (size_t i = 0; i < size - 1; ++i) {
        for (size_t j = 0; j < size - i - 1; ++j) {
            if (dataset[j] > dataset[j + 1]) {
                std::swap(dataset[j], dataset[j + 1]);
            }
        }
    }
}

void bubbleSortFlag(std::vector<int>& dataset){
    const size_t size = dataset.size();

    for (int i = 0; i < size - 1; i++){
        bool swapped = false;

        for (int j = 0; j < size - i - 1; j++){
            if (dataset[j] > dataset[j + 1]){
                std::swap(dataset[j], dataset[j + 1]);
                swapped = true;
            }
        }

        if (!swapped)
            break;
    }

}

void bubbleSortParallel(std::vector<int>& dataset) {
    const size_t size = dataset.size();

    for (size_t k = 0; k < size - 1; ++k) {
        #pragma omp parallel for default(none) shared(dataset, size, k)
        for (size_t i = k % 2; i < size- 1; i += 2)
            if (dataset[i] > dataset[i + 1])
                std::swap(dataset[i], dataset[i + 1]);
    }
}


void bubbleSortParallelFlag(std::vector<int>& dataset) {
    const size_t size = dataset.size();
    bool swapped = true;

    for (size_t k = 0; swapped && k < size - 1; ++k) {
        swapped = false;

        #pragma omp parallel reduction(|:swapped)
        {
            #pragma omp for
            for (size_t i = k % 2; i < size - 1; i += 2) {
                if (dataset[i] > dataset[i + 1]) {
                    std::swap(dataset[i], dataset[i + 1]);
                    swapped = true;
                }
            }
        }
    }
}

int main()
{
    constexpr int DATA_SIZE = 10000;
    constexpr int NUM_THREADS = 8;
    constexpr int TEST_QUANTITY = 100;
    constexpr float BASE_TIME = 0.420109;

    omp_set_num_threads(NUM_THREADS);

    double average = 0;
    std::vector<int> dataset(DATA_SIZE);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000000);

    for (int i = 0; i < TEST_QUANTITY; ++i) {
        for (auto& val : dataset) {
            val = dis(gen);
        }

        std::cout << "\rTest repetition: " << i + 1 << " / " << TEST_QUANTITY << std::flush;

        const auto start = std::chrono::high_resolution_clock::now();
        // bubbleSort(dataset);
        // bubbleSortFlag(dataset);
        bubbleSortParallel(dataset);
        // bubbleSortParallelFlag(dataset);
        const auto end = std::chrono::high_resolution_clock::now();
        const std::chrono::duration<double> elapsed = end - start;

        average += elapsed.count();
    }

    average /= TEST_QUANTITY;

    std::cout << "\n";
    std::cout << "Average time taken to sort with " << DATA_SIZE << " dataset and " << NUM_THREADS <<" of cores: " << average << " seconds\n";
    std::cout << "Speedup: " << average/BASE_TIME << "\n";

    return 0;
}
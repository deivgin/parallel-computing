#include <vector>
#include <random>
#include <iostream>
#include <chrono>

std::vector<int> generateLargeDataset(const size_t size, const int min = 1, const int max = 1000000) {
    std::vector<int> dataset(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);

    for (size_t i = 0; i < size; ++i) {
        dataset[i] = dis(gen);
        std::cout << "\rGenerating dataset: Index " << i + 1 << " of " << size;
        std::cout.flush();
    }
    std::cout << "\n";

    return dataset;
}

void sequentialBubbleSort(std::vector<int>& arr) {
    const size_t n = arr.size();

    for (size_t i = 0; i < n - 1; ++i) {
        for (size_t j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                const int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }

        // Display progress in the same line
        std::cout << "\rSorting dataset: Pass " << i + 1 << " of " << n;
        std::cout.flush();
    }

    std::cout << "\n";
}

int main() {
    constexpr size_t size = 100000;
    std::vector<int> dataset = generateLargeDataset(size);

    const auto start = std::chrono::high_resolution_clock::now();
    sequentialBubbleSort(dataset);
    const auto end = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> elapsed = end - start;

    std::cout << "Time taken to sort the dataset: " << elapsed.count() << " seconds\n";

    return 0;
}
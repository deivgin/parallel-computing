#include <vector>
#include <random>
#include <iostream>

std::vector<int> generateLargeDataset(const size_t size, const int min = 1, const int max = 1000000) {
    std::vector<int> dataset(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);

    for (size_t i = 0; i < size; ++i) {
        dataset[i] = dis(gen);
    }

    return dataset;
}

int main() {
    constexpr size_t size = 100000;
    const std::vector<int> dataset = generateLargeDataset(size);

    std::cout << "Generated dataset (first 10 elements): ";
    for (size_t i = 0; i < 10 && i < dataset.size(); ++i) {
        std::cout << dataset[i] << " ";
    }
    std::cout << "\n";

    return 0;
}
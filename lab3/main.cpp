#include <vector>
#include <iostream>
#include <chrono>
#include <random>
#include <omp.h>
#include <fstream>
#include <sstream>

std::vector<int> generateDataset(const size_t size, const int min = 1, const int max = 1000000)
{
    std::vector<int> dataset(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);

    for (size_t i = 0; i < size; ++i)
    {
        dataset[i] = dis(gen);
    }

    return dataset;
}

void sequentialBubbleSort(std::vector<int>& arr)
{
    const size_t n = arr.size();

    for (int i = 0; i < n - 1; i++)
    {
        bool swapped = false;

        for (int j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }

        if (!swapped)
            break;
    }

}

void parallelBubbleSort(std::vector<int>& arr)
{
    const size_t n = arr.size();
    bool swapped = true;

    for (size_t k = 0; swapped && k < n - 1; ++k)
    {
        swapped = false;

        #pragma omp parallel num_threads(1)
        {
            bool localSwapped = false;

            #pragma omp for
            for (size_t i = k % 2; i < n - 1; i += 2)
            {
                if (arr[i] > arr[i + 1])
                {
                    std::swap(arr[i], arr[i + 1]);
                    localSwapped = true;
                }
            }

            #pragma omp atomic
            swapped |= localSwapped;
        }
    }
}

int main()
{
    constexpr int DATA_SIZE = 1'000'000;

        std::vector<int> dataset = generateDataset(DATA_SIZE);

        const auto start = std::chrono::high_resolution_clock::now();
        parallelBubbleSort(dataset);
        const auto end = std::chrono::high_resolution_clock::now();
        const std::chrono::duration<double> elapsed = end - start;

        std::cout << "Time taken to sort with " << DATA_SIZE << " dataset: " << elapsed.count() << " seconds\n";

    return 0;
}
#include <iostream>
#include <vector>

void bubbleSort(std::vector<int>& arr) {
    const unsigned long n = arr.size();

    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;

        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }

        if (!swapped)
            break;
    }
}

int main() {
    std::vector<int> arr = {64, 34, 25, 12, 22, 11, 90};

    std::cout << "Original array: ";
    for (const int num : arr)
        std::cout << num << " ";
    std::cout << std::endl;

    bubbleSort(arr);

    std::cout << "Sorted array: ";
    for (const int num : arr)
        std::cout << num << " ";
    std::cout << std::endl;

    return 0;
}


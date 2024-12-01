#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double measureExecutionTime(void (*func)(int[], int), int arr[], int size) {
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);
    func(arr, size);
    clock_gettime(CLOCK_MONOTONIC, &end);

    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

void insertionSort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        const int key = arr[i];
        int j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

int* generateRandomArray(const int size, const int max_value) {
    int* arr = (int*)malloc(size * sizeof(int));
    if (arr == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(1);
    }

    for (int i = 0; i < size; i++) {
        arr[i] = rand() % max_value;
    }
    return arr;
}

int main() {
    srand(time(NULL));

    const int size = 300000;
    const int max_value = 10000;

    int* arr = generateRandomArray(size, max_value);

    const double time_taken = measureExecutionTime(insertionSort, arr, size);

    printf("Sorted %d elements\n", size);
    printf("Execution time: %.9f seconds\n", time_taken);

    for (int i = 1; i < size; i++) {
        if (arr[i] < arr[i-1]) {
            printf("Sorting failed at index %d\n", i);
            break;
        }
    }

    free(arr);
    
    return 0;
}
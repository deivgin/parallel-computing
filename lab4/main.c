#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

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

void merge(int* arr1, int n1, int* arr2, int n2, int* result) {
    int i = 0, j = 0, k = 0;

    while (i < n1 && j < n2) {
        if (arr1[i] <= arr2[j]) {
            result[k++] = arr1[i++];
        } else {
            result[k++] = arr2[j++];
        }
    }

    while (i < n1) result[k++] = arr1[i++];
    while (j < n2) result[k++] = arr2[j++];
}

int* generateRandomArray(int size, int max_value) {
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

int main(int argc, char** argv) {
    // Initialize MPI
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Configuration parameters
    const int total_size = 10000;     // Total array size
    const int max_value = 10000;      // Maximum value for random numbers

    // Calculate local array size
    const int local_size = total_size / size;
    const int remainder = total_size % size;
    const int my_size = (rank == size - 1) ? local_size + remainder : local_size;

    // Root process generates data
    int* global_array = NULL;
    if (rank == 0) {
        srand(time(NULL));
        global_array = generateRandomArray(total_size, max_value);
        printf("Generated %d random numbers\n", total_size);
    }

    // Allocate local array
    int* local_array = (int*)malloc(my_size * sizeof(int));
    if (local_array == NULL) {
        fprintf(stderr, "Process %d: Memory allocation failed\n", rank);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // Start timing
    double start_time = MPI_Wtime();

    // Distribute data
    int* sendcounts = (int*)malloc(size * sizeof(int));
    int* displs = (int*)malloc(size * sizeof(int));

    for (int i = 0; i < size; i++) {
        sendcounts[i] = (i == size - 1) ? local_size + remainder : local_size;
        displs[i] = i * local_size;
    }

    MPI_Scatterv(global_array, sendcounts, displs, MPI_INT,
                 local_array, my_size, MPI_INT,
                 0, MPI_COMM_WORLD);

    // Sort local array
    insertionSort(local_array, my_size);

    // Gather and merge results
    if (rank == 0) {
        int* temp = (int*)malloc(total_size * sizeof(int));
        if (temp == NULL) {
            fprintf(stderr, "Root process: Memory allocation failed\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        // Copy root's sorted array
        for (int i = 0; i < my_size; i++) {
            global_array[i] = local_array[i];
        }

        // Receive and merge from other processes
        int current_size = my_size;
        for (int i = 1; i < size; i++) {
            MPI_Recv(temp, sendcounts[i], MPI_INT, i, 0,
                    MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            merge(global_array, current_size, temp, sendcounts[i], global_array);
            current_size += sendcounts[i];
        }

        free(temp);

        // Stop timing
        double end_time = MPI_Wtime();
        printf("Parallel execution time: %f seconds\n", end_time - start_time);

        // Verify sorting
        for (int i = 1; i < total_size; i++) {
            if (global_array[i] < global_array[i-1]) {
                printf("Sorting failed at index %d\n", i);
                break;
            }
        }
    } else {
        // Send sorted local array to root
        MPI_Send(local_array, my_size, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    // Cleanup
    if (rank == 0) free(global_array);
    free(local_array);
    free(sendcounts);
    free(displs);

    MPI_Finalize();
    return 0;
}
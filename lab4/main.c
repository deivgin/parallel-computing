#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <mpi.h>

void insertionSort(int arr[], const int n) {
    for (int i = 1; i < n; i++) {
        const int key = arr[i];
        int j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void merge(int arr[], const int left[], const int right[], const int left_size, const int right_size) {
    int i = 0, j = 0, k = 0;

    while (i < left_size && j < right_size) {
        if (left[i] <= right[j]) {
            arr[k++] = left[i++];
        } else {
            arr[k++] = right[j++];
        }
    }

    while (i < left_size) {
        arr[k++] = left[i++];
    }

    while (j < right_size) {
        arr[k++] = right[j++];
    }
}

void generateRandomArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 10000;
    }
}

double runSequentialSort(const int n) {
    int *arr = malloc(n * sizeof(int));
    generateRandomArray(arr, n);

    const double start_time = MPI_Wtime();
    insertionSort(arr, n);
    const double end_time = MPI_Wtime();

    free(arr);
    return end_time - start_time;
}

double runParallelSort(int n, int rank, int size) {
    const int local_n = n / size;
    int *data = NULL;
    int *local_data = malloc(local_n * sizeof(int));
    int *gathered_data = NULL;

    if (rank == 0) {
        data = malloc(n * sizeof(int));
        gathered_data = malloc(n * sizeof(int));
        generateRandomArray(data, n);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    const double start_time = MPI_Wtime();

    MPI_Scatter(data, local_n, MPI_INT, local_data, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    insertionSort(local_data, local_n);

    MPI_Gather(local_data, local_n, MPI_INT, gathered_data, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        int *temp = malloc(n * sizeof(int));
        for (int i = 0; i < size - 1; i++) {
            merge(temp, gathered_data, gathered_data + (i + 1) * local_n,
                  (i + 1) * local_n, local_n);
            for (int j = 0; j < (i + 2) * local_n; j++) {
                gathered_data[j] = temp[j];
            }
        }
        free(temp);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    const double end_time = MPI_Wtime();

    if (rank == 0) {
        free(data);
        free(gathered_data);
    }
    free(local_data);

    return end_time - start_time;
}

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int n = 100000;
    double sequential_time = 0;
    double parallel_time = 0;

    if (rank == 0) {
        sequential_time = runSequentialSort(n);
        printf("\nArray size: %d\n", n);
        printf("Number of processes: %d\n", size);
        printf("Sequential time: %f seconds\n", sequential_time);
    }

    parallel_time = runParallelSort(n, rank, size);

    if (rank == 0) {
        printf("Parallel time: %f seconds\n", parallel_time);
        printf("Speedup: %f\n", sequential_time / parallel_time);
    }

    MPI_Finalize();
    return 0;
}
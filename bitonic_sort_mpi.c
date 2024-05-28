#include <time.h>
// #include <omp.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <mpi.h>

// this will swap the two values, they will point to their respective values
void swap(int *p1, int *p2)
{
    int temp;
    temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}

int partition(int arr[], int low, int high)
{
    // choose the last elemeent as the pivot element
    int pivot = arr[high];

    // this will tell use where the pivot element is going to be placed
    int i = (low - 1);

    for (int j = low; j <= high; j++)
    {
        if (arr[j] < pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    // this is swapping the pivot element with the element at i + 1
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(int arr[], int low, int high)
{
    // when low is less than high
    if (low < high)
    {
        // pi is the partition return index of pivot

        int pi = partition(arr, low, high);

        // Recursion Call
        // smaller element than pivot goes left and
        // higher element goes right

        // we know that the pivot element is at the right place and everything to the left of the pivot is less than the pivot
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

bool isSorted(int arr[], int n)
{
    for (int i = 1; i < n; i++)
    {
        if (arr[i] < arr[i - 1])
        {
            return false;
        }
    }
    return true;
}

// fisrt i must convert an array into a bitonic sequence
void compAndSwap(int a[], int i, int j, bool dir)
{
    if (dir == (a[i] > a[j]))
        swap(&a[i], &a[j]);
}

/*It recursively sorts a bitonic sequence in ascending order,
  if dir = 1, and in descending order otherwise (means dir=0).
  The sequence to be sorted starts at index position low,
  the parameter cnt is the number of elements to be sorted.*/
void bitonicMerge(int a[], int low, int cnt, bool dir)
{
    if (cnt > 1)
    {
        int k = cnt / 2;
        for (int i = low; i < low + k; i++)
            compAndSwap(a, i, i + k, dir);
        bitonicMerge(a, low, k, dir);
        bitonicMerge(a, low + k, k, dir);
    }
}

/* This function first produces a bitonic sequence by recursively
    sorting its two halves in opposite sorting orders, and then
    calls bitonicMerge to make them in the same order */

void bitonicSort(int a[], int low, int cnt, bool dir)
{

    if (cnt > 1)
    {
        int k = cnt / 2;
        bitonicSort(a, low, k, 1);
        bitonicSort(a, low + k, k, 0);
        bitonicMerge(a, low, cnt, dir);
    }
}

int main(int argc, char *argv[])
{

    int p = atoi(argv[1]);
    int size = pow(2, p);
    int *arr_serial = (int *)malloc(size * sizeof(int));
    int *arr_MPI = (int *)malloc(size * sizeof(int));
    int *array = (int *)malloc(size * sizeof(int));
    if (arr_serial == NULL || arr_MPI == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    srand(time(0));
    for (int i = 0; i < size; i++)
    {
        array[i] = rand() % size + 1;
        arr_serial[i] = array[i];
        arr_MPI[i] = array[i];
    }
    int n = size;
    double q_time;
    // double start, end;
    // double total_time = 0.0;
    // int num_runs = 1;
    int rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0)
    {
        double start, end;
        start = MPI_Wtime();

        quickSort(arr_serial, 0, n);

        end = MPI_Wtime();

        double total_time = end - start;
        q_time = total_time;
        printf("Time taken by SErial: %f\n", total_time);
        isSorted(arr_serial, n) ? printf("Sorted\n") : printf("Not Sorted\n");
    }
    int s;
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Comm_size(MPI_COMM_WORLD, &s);
    double start, end;

    if (rank == 0)
    {
        start = MPI_Wtime();
    }

    int segment = n / s;
    int low = rank * segment;
    int high = segment;
    bool dir = rank % 2 == 0 ? 1 : 0;
    bitonicSort(arr_MPI, low, high, dir);
    MPI_Allgather(arr_MPI + low, high, MPI_INT, arr_MPI, high, MPI_INT, MPI_COMM_WORLD);
    if (rank == 0)
    {

        bitonicMerge(arr_MPI, low, segment * 2, 1);
    }
    else if (rank == 2)
    {
        bitonicMerge(arr_MPI, segment * 2, segment * 2, 0);
    }
    MPI_Bcast(arr_MPI, segment * 2, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(arr_MPI + segment * 2, segment * 2, MPI_INT, 2, MPI_COMM_WORLD);

    if (rank == 0)
    {
       
        int k = n / 2;

        for (int i = low; i < low + k; i++)
            compAndSwap(arr_MPI, i, i + k, 1);

        
        k = k / 2;
        // this is wrong
        for (int i = low; i < low + k; i++)
            compAndSwap(arr_MPI, i, i + k, 1);
        
        for (int i = segment * 2; i < segment * 2 + k; i++)
            compAndSwap(arr_MPI, i, i + k, 1);
      
    }
    // MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(arr_MPI, n, MPI_INT, 0, MPI_COMM_WORLD);
    bitonicMerge(arr_MPI, low, high, 1);
    MPI_Allgather(arr_MPI + low, high, MPI_INT, arr_MPI, high, MPI_INT, MPI_COMM_WORLD);
    // printf("rank: %d\n", rank);
    // printf("last: ");
    // for (int i = 0; i < n; i++)
    // {
    //     printf("%d ", arr_MPI[i]);
    // }
    printf("\n");
    if (rank == 0)
    {
        end = MPI_Wtime();
        double total_time = end - start;
        printf("Time taken by MPI: %f\n", total_time);

        isSorted(arr_MPI, n) ? printf("Sorted\n") : printf("Not Sorted\n");
        printf("Speedup: %f\n", q_time / total_time);

        printf("\n");
    }

    free(arr_MPI);
    free(array);
    MPI_Finalize();
    return 0;
}

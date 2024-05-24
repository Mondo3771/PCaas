#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
int isSorted(int arr[], int n)
{
    for (int i = 1; i < n; i++)
    {
        if (arr[i] < arr[i -1])
        {
            return 0; // Not sorted
        }
    }
    return 1; // Sorted
}
void swap(int *p1, int *p2)
{
    int temp;
    temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}

int partition(int arr[], int low, int high)
{
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high; j++)
    {
        if (arr[j] < pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(int arr[], int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

int main()
{
    int size = pow(2,16);
    int arr[size];
    srand(time(0));
    for (int i = 0; i < size; i++)
    {
        arr[i] = rand() % size + 1;
    }
    double start, end;
    start = clock();
    int n = sizeof(arr) / sizeof(arr[0]);
    quickSort(arr, 0, n - 1);
    end = clock();
    isSorted(arr, n) ? printf("Array is sorted\n") : printf("Array is not sorted\n");
    printf("Sorted Array\n");
    printf("Time taken: %f\n", (end - start) / CLOCKS_PER_SEC);
   
    return 0;
}

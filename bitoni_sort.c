#include <time.h>
#include <omp.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

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
// #pragma omp parallel for schedule(s, 64)
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

		// sort in ascending order since dir here is 1
		bitonicSort(a, low, k, 1);
		// sort in descending order since dir here is 0
		bitonicSort(a, low + k, k, 0);

		// Will merge whole sequence in ascending order
		// since dir=1.

		bitonicMerge(a, low, cnt, dir);
	}
}

int main(int argc, char *argv[])
{
	int p = atoi(argv[1]);
	int size = pow(2, p);
	int arr_serial[size];
	int arr_Open_mp[size];
	int arr_MPI[size];
	srand(time(0));
	//  this is just creating the array i am going to sort
	for (int i = 0; i < size; i++)
	{
		arr_serial[i] = rand() % size + 1;
		arr_Open_mp[i] = arr_serial[i];
		arr_MPI[i] = arr_serial[i];
	}

	double start, end;
	start = omp_get_wtime();
	// this is giving use the size of the array
	int n = sizeof(arr_serial) / sizeof(arr_serial[0]);
	quickSort(arr_serial, 0, n - 1);
	end = omp_get_wtime();
	isSorted(arr_serial, n) ? printf("Sorted\n") : printf("Not Sorted\n");
	printf("QuickSort Time taken: %f\n", end - start);
	double q_time = end - start;

	start = omp_get_wtime();
	bitonicSort(arr_Open_mp, 0, n, 1);

	end = omp_get_wtime();
	isSorted(arr_Open_mp, n) ? printf("Sorted\n") : printf("Not Sorted\n");
	double b_time = end - start;
	printf("Bitonic Time taken: %f\n", end - start);
	double Speedup = (q_time) / (b_time);

	printf("Speedup: %f\n", Speedup);
}

#include <iostream>
using namespace std;

void sort(int arr[], int size, bool ascending);
void bitonicSort(int arr[], int low, int cnt, bool direction);
void bitonicMerge(int arr[], int low, int size, bool direction);
void compare_swap(int arr[], int i, int j, bool direction);
int greatest_power_of_two_less_than(int n);

int main() {
    int n;
    cout << "Enter the size of the array " << endl;
    cin >> n;
    int arr[n];

    // Input array elements
    for(int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    // Sort array elements
    bool ascending = true;
    sort(arr, n, ascending);

    // Print sorted array elements
    for(int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
    return 0;
}

// Call bitonic sort for sorting entire array of length N in ASCENDING order
void sort(int arr[], int size, bool ascending) {
    bitonicSort(arr, 0, size, ascending);
    return;
}

// Produce bitonic sequence and then merge them recursively
void bitonicSort(int arr[], int low, int cnt, bool direction) {
    if(cnt > 1) {
        int k = cnt / 2;
        // sorting in descending order
        bitonicSort(arr, low, k, !direction);
        
        // sorting in ascending order
        bitonicSort(arr, low+k, cnt-k, direction);

        // merge the sequence
        bitonicMerge(arr, low, cnt, direction);
    }
    return;
}

// Merge
void bitonicMerge(int arr[], int low, int size, bool direction) {
    if(size > 1) {
        int m = greatest_power_of_two_less_than(size);
        for(int i = low; i < low+size-m; i++) {
            compare_swap(arr, i, i+m, direction);
        }
        bitonicMerge(arr, low, m, direction);
        bitonicMerge(arr, low+m, size-m, direction);
    }
    return;
}

// Compare and swap elements of array
void compare_swap(int arr[], int i, int j, bool direction) {
    if(direction ==(arr[i] > arr[j]))
        swap(arr[i], arr[j]);
    return;
}

// n>=2 and n<=MAX_INT
int greatest_power_of_two_less_than(int n) {
    int k = 1;
    while(k > 0 && k < n) {
        k = k << 1;
    }
    return k>>1;
}
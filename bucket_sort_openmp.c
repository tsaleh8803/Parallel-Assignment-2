#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define num_threads 10
#define ARRAYSIZE 100000
#define MAX_VALUE 100000

void* insertion_sort(int* arr, size_t size) {
    for (size_t i = 1; i < size; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

int main(int argc, char* argv[]) {
double trials[10];
for(int k = 0;k<10;k++){

	int a[ARRAYSIZE];
	omp_set_num_threads(num_threads);
	int tid;
	
	// Generate random numbers for the array
	#pragma omp parallel for
	for (int i = 0; i < ARRAYSIZE; i++) {
		a[i] = rand() % (MAX_VALUE + 1); // Generate random numbers between 0 and MAX_VALUE
	}

	int partition = (MAX_VALUE + 1) / num_threads;
	int* sorted_subarrays[num_threads];
	int subarray_sizes[num_threads];
	
	clock_t start = clock();
	// Begin of parallel region
	#pragma omp parallel private(tid)
	{
	int tid = omp_get_thread_num();
	int start = tid * partition;
	int end = start + partition;

	// Allocate memory for the thread's array
	int* thread_array = (int*)malloc(ARRAYSIZE * sizeof(int));
	int t = 0;

	// Filter elements within the range
	for (int j = 0; j < ARRAYSIZE; j++) {
	    if (a[j] > start && a[j] <= end) {
		thread_array[t++] = a[j];
	    }
	}

	int *thread_array_without_zeros = (int*)malloc(t * sizeof(int));
	memcpy(thread_array_without_zeros, thread_array, t * sizeof(int));

	// Sort the thread's array
	insertion_sort(thread_array_without_zeros, t);

	sorted_subarrays[tid] = thread_array_without_zeros;
	subarray_sizes[tid] = t;

	// Free memory for the thread's array
	free(thread_array);

	}

	int *sorted_array = (int*)malloc(ARRAYSIZE * sizeof(int));
	   
	int index = 0;
	for (int i = 0; i < num_threads; i++) {
		int *subarray = sorted_subarrays[i];
		int length = subarray_sizes[i];
		for (int j = 0; j < length; j++) {
		    sorted_array[index++] = subarray[j];
		}
		free(subarray); // Free memory for each sub-array
	}
	clock_t end = clock();
	// Print sorted array
	    //printf("Sorted array: ");
	    //for (size_t i = 0; i < ARRAYSIZE; i++) {
		//printf("%d ", sorted_array[i]);
	    //}
	    double p_time = ((double)end - start)/ CLOCKS_PER_SEC;
	    printf("OpenMP Parallel time: %f\n",p_time);
	    trials[k] = p_time;
	}
	
	double sum= 0;
	for(int j = 0;j<10;j++){
		sum+= trials[j];
	}
	printf("Average Parallel time: %f",sum/10);	    
    return 0;
}
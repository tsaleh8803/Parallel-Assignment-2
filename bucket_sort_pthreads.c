#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define num_threads 2
#define ARRAYSIZE 100000
#define MAX_VALUE 100000

//Each thread will take its section of the array 
//each thread corresponds to a range it needs to sort within


// Structure to pass arguments to the thread function
typedef struct {
    int *array;
    size_t size;
} ThreadArgs;

// Sorting function for each thread to sort sub-array
void* sort_array(void *arg) {
    ThreadArgs *args = (ThreadArgs*)arg;
    int *array = args->array;
    size_t size = args->size;
    
    //printf("Thread Sub-array: ");
    //for (size_t i = 0; i < size; i++) {
	//printf("%d ", array[i]);
    //}
    //printf("\n");

    // Allocate memory for the thread's array, ignoring zeros
    int *thread_array = (int*)malloc(size * sizeof(int));
    int t = 0;
    for (size_t i = 0; i < size; i++) {
        if (array[i] != 0) {
            thread_array[t++] = array[i];
        }
    }
    size = t; // Update size to account for only non-zero elements

    // Sorting algorithm (Insertion sort)
    for (size_t i = 1; i < size; i++) {
    	int key = thread_array[i];
    	int j = i - 1;
    	while (j >= 0 && thread_array[j] > key) {
        	thread_array[j + 1] = thread_array[j];
        	j = j - 1;
    	}
    	thread_array[j + 1] = key;
	}
    
    //printf("Sorted sub-array: ");
    //for (size_t i = 0; i < size; i++) {
	//printf("%d ", thread_array[i]);
    //}
    //printf("\n");

    // Return the sorted array
    return thread_array;
}

int findMax(int arr[], int size) {
    int max = arr[0]; // Assume the first element as maximum

    // Iterate through the array to find the maximum element
    for (int i = 1; i < size; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }

    return max;
}

int main() {
	double trials[10];
	for(int k =0;k<10;k++){
	
		int a[ARRAYSIZE];  
		for (int i = 0; i < ARRAYSIZE; i++) {
			a[i] = rand() % (MAX_VALUE + 1); // Generate random numbers between 0 and MAX_VALUE
	    	}
		int partition = (MAX_VALUE+1)/num_threads;
		
		pthread_t thread_arr[num_threads];
		ThreadArgs thread_args[num_threads];
		
		// Array to hold sorted sub-arrays
	   	int* sorted_subarrays[num_threads];
		
		clock_t start = clock();
		
		// Assign elements to each thread's array
	    	for (int i = 0; i < num_threads; i++) {
			int start = i * partition;
			int end = start + partition;

			// Allocate memory for the thread's array
			int* thread_array = (int*)malloc(ARRAYSIZE * sizeof(int));
			
			//Distributing the numbers to each bucket based on range of thread
			int t = 0;
			for(int j = 0;j<ARRAYSIZE;j++){
				if(a[j]>start && a[j]<=end ){
					thread_array[t++] = a[j];
				}
			}
			
			
			thread_args[i].array = thread_array;
			thread_args[i].size = t;
			
			pthread_create(&thread_arr[i], NULL, sort_array, &thread_args[i]);

			// Process the elements in the thread's array (you can pass it to a thread for processing)
			//printf("Thread %d: Range (%d, %d]\n", i, start, end);
			
	    	}
	    	
	    	for( int i = 0; i<num_threads;i++){
	    		pthread_join(thread_arr[i], (void**)&sorted_subarrays[i]);
	    	}
	 	
	    	int *sorted_array = (int*)malloc(ARRAYSIZE * sizeof(int));
	    	
	    	int index = 0;
		for (int i = 0; i < num_threads; i++) {
			int *subarray = sorted_subarrays[i];
			int length = thread_args[i].size;
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
		    printf("\n");
		    double pthread_time = ((double)end - start)/ CLOCKS_PER_SEC;
	  	    printf("PThread Parallel time: %f\n",pthread_time);
	  	    trials[k] = pthread_time;
  	    }
    	double sum= 0;
	for(int j = 0;j<10;j++){
		sum+= trials[j];
	}
	printf("Average Parallel time: %f",sum/10);
	return 0;
}
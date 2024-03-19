//SEQUENTIAL CODE TAKEN FROM PROGRAMIZ.COM/DSA/BUCKET-SORT

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NARRAY 100000   // Array size
#define NBUCKET 2  // Number of buckets
#define INTERVAL 50000 // Each bucket capacity
#define MAX_VALUE 100000

struct Node {
  int data;
  struct Node *next;
};

void BucketSort(int arr[]);
struct Node *InsertionSort(struct Node *list);
void print(int arr[]);
void printBuckets(struct Node *list);
int getBucketIndex(int value);

// Sorting function
void BucketSort(int arr[]) {
  int i, j;
  struct Node **buckets;

  // Create buckets and allocate memory size
  buckets = (struct Node **)malloc(sizeof(struct Node *) * NBUCKET);

  // Initialize empty buckets
  for (i = 0; i < NBUCKET; ++i) {
    buckets[i] = NULL;
  }

  // Fill the buckets with respective elements
  for (i = 0; i < NARRAY; ++i) {
    struct Node *current;
    int pos = getBucketIndex(arr[i]);
    current = (struct Node *)malloc(sizeof(struct Node));
    current->data = arr[i];
    current->next = buckets[pos];
    buckets[pos] = current;
  }

  // Print the buckets along with their elements
  //for (i = 0; i < NBUCKET; i++) {
    //printf("Bucket[%d]: ", i);
    //printBuckets(buckets[i]);
    //printf("\n");
  //}

  // Sort the elements of each bucket
  for (i = 0; i < NBUCKET; ++i) {
    buckets[i] = InsertionSort(buckets[i]);
  }

  printf("-------------\n");
  //printf("Bucktets after sorting\n");
  //for (i = 0; i < NBUCKET; i++) {
    //printf("Bucket[%d]: ", i);
    //printBuckets(buckets[i]);
  //printf("\n");
  //}

  // Put sorted elements on arr
  for (j = 0, i = 0; i < NBUCKET; ++i) {
    struct Node *node;
    node = buckets[i];
    while (node) {
      arr[j++] = node->data;
      node = node->next;
    }
  }

  return;
}

// Function to sort the elements of each bucket
struct Node *InsertionSort(struct Node *list) {
  struct Node *k, *nodeList;
  if (list == 0 || list->next == 0) {
    return list;
  }

  nodeList = list;
  k = list->next;
  nodeList->next = 0;
  while (k != 0) {
    struct Node *ptr;
    if (nodeList->data > k->data) {
      struct Node *tmp;
      tmp = k;
      k = k->next;
      tmp->next = nodeList;
      nodeList = tmp;
      continue;
    }

    for (ptr = nodeList; ptr->next != 0; ptr = ptr->next) {
      if (ptr->next->data > k->data)
        break;
    }

    if (ptr->next != 0) {
      struct Node *tmp;
      tmp = k;
      k = k->next;
      tmp->next = ptr->next;
      ptr->next = tmp;
      continue;
    } else {
      ptr->next = k;
      k = k->next;
      ptr->next->next = 0;
      continue;
    }
  }
  return nodeList;
}

int getBucketIndex(int value) {
  return value / INTERVAL;
}

void print(int ar[]) {
  int i;
  for (i = 0; i < NARRAY; ++i) {
    printf("%d ", ar[i]);
  }
  printf("\n");
}

// Print buckets
void printBuckets(struct Node *list) {
  struct Node *cur = list;
  while (cur) {
    printf("%d ", cur->data);
    cur = cur->next;
  }
}

int main(void) {

	double trials[10];

	for(int k = 0;k<10;k++){

	  int array[NARRAY];
	  
	  for (int i = 0; i < NARRAY; i++) {
		array[i] = rand() % (MAX_VALUE + 1); // Generate random numbers between 0 and MAX_VALUE
	   }

	  //printf("Initial array: ");
	  //print(array);
	  //printf("-------------\n");
	  
	  clock_t start = clock();

	  BucketSort(array);
	  clock_t end = clock();
	  //printf("-------------\n");
	  printf("Sorted array: ");
	  print(array);
	  double seq_time = ((double)end - start)/ CLOCKS_PER_SEC;
	  trials[k] = seq_time;
	  printf("Sequential time %d: %f\n",k, trials[k]);
	  
	}
	double sum= 0;
	for(int j = 0;j<10;j++){
		sum+= trials[j];
	}
	printf("Average Sequential time: %f",sum/10);
	return 0;
}
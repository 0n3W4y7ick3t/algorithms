/* quick sort algorithm */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int swap_count = 0;
void swap(int *a, int *b) {
  swap_count++;
  int temp = *a;
  *a = *b;
  *b = temp;
}

void print_arr(int arr[], int len) {
  printf("[");
  for (int i = 0; i < len - 1; i++)
    printf("%d, ", arr[i]);
  printf("%d]\n", arr[len - 1]);
}

/* [lo, hi) */
int foo_partition(int arr[], int lo, int hi) {
  int pivot = arr[lo];
  int i = lo + 1;
  int j = lo + 1;
  while (1) {
    while (arr[j] <= pivot && j < hi) {
      i++;
      j++;
      /* i is the first item biger than pivot */
    }
    /* skip following items which are bigger than pivot */
    while (arr[j] >= pivot && j < hi)
      j++;
    /* j is the first item sould be move to the left(it's lesser than pivot) */
    if (j == hi)
      break;

    /* printf("swapping %2d @%2d with %2d @%2d\n", arr[i], i, arr[j], j); */
    swap(arr + i, arr + j);
    i++;
    j++;
  }
  if (lo != i - 1) {
    /* printf("swapping pivot(%d) with %2d @%2d\n", arr[lo], arr[i - 1], i - 1);
     */
    swap(arr + lo, arr + i - 1);
  }
  /* printf("sub array: "); */
  /* print_arr(arr + lo, hi - lo); */
  return i - 1;
}

/* [lo, hi) */
int hoare(int arr[], int lo, int hi) {
  int pivot = arr[lo];
  int i = lo, j = hi - 1;
  while (1) {
    while (arr[i] < pivot)
      i++;
    while (arr[j] > pivot)
      j--;
    if (i >= j)
      return j;
    swap(arr + i, arr + j);
  }
}

/* [lo, hi) */
int lomuto(int arr[], int lo, int hi) {
  int pivot = arr[lo];
  int i = lo;
  for (int j = lo + 1; j < hi; j++) {
    if (arr[j] <= pivot) {
      i++;
      if (i != j) {
        printf("swapping %2d @%2d with %2d @%2d\n", arr[i], i, arr[j], j);
        swap(arr + i, arr + j);
      }
    }
  }
  printf("swapping pivot(%d) with %2d @%2d\n", arr[lo], arr[i], i);
  swap(arr + lo, arr + i);
  return i;
}

typedef int partitioner(int[], int, int);
void quick_sort(int arr[], int low, int high, partitioner part) {
  if (low < high) {
    int mid = part(arr, low, high);
    quick_sort(arr, low, mid, part);
    quick_sort(arr, mid + 1, high, part);
  }
}

int main() {
  int arr[] = {8, 3, 1, 5, 10, 14, 11, -3, 90};
  int len = sizeof(arr) / sizeof(arr[0]);
  srand(time(NULL));
  printf("Original: ");
  print_arr(arr, len);

  quick_sort(arr, 0, len, hoare);
  printf("Result: ");
  print_arr(arr, len);
  printf("swapped %d times.", swap_count);
  return 0;
}

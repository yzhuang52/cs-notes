#include <stdio.h>
#include <string.h>

void swap(char* a, int i, int j) {
  char t = a[i];
  a[i] = a[j];
  a[j] = t;
}

int partition(char* a, int l, int r){
  int pivot = a[l];
  int i = l, j = r+1;

  while (1) {
    do
      ++i;
    while (a[i] <= pivot && i <= r);

    do
      --j;
    while (a[j] > pivot);

    if (i >= j)
      break;

    swap(a, i, j);
  }

  swap(a, l, j);

  return j;
}

void sort(char* a, int l, int r){
  if (l < r){
    int j = partition(a, l, r);
    sort(a, l, j-1);
    sort(a, j+1, r);
  }
}

int main(int argc, char** argv){
  char* a = NULL;

  if (argc > 1)
    a = argv[1];
  else {
    a = "IU is the best singer!";
  }
  printf("Unsorted: \"%s\"\n", a);
  sort(a, 0, strlen(a) - 1);
  printf("Sorted  : \"%s\"\n", a);
}
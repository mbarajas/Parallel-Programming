#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 2048

void main() {
  int c;
  double *firstArray = malloc(SIZE * SIZE * sizeof(double));
  double *secondArray = malloc(SIZE * SIZE * sizeof(double));
  double *sum = malloc(SIZE * SIZE * sizeof(double));

  int i = 0;
  for(i = 0; i < SIZE * SIZE; i++) {
    firstArray[i] = 2;
    secondArray[i] = 2;
    sum[i] = 0;
  }
  int j = 0;
  int k = 0;
  c = clock();
  for (i = 0; i < SIZE; i++) {
    for (j = 0; j < SIZE; j++) {
      double elementSum = 0;
      for (k = 0; k < SIZE; k++)
       elementSum += firstArray[i * SIZE + k] + secondArray[k * SIZE + j];
      sum[i * SIZE + j] = elementSum;
    }
  }
  printf("Done. Time: %d\n", clock() - c);
}

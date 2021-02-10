#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int sum_array(int list[], int num_elements);

void main(void)
{
  int i = 0;
  int list[10];
  for(i = 0; i < 10; i++){
   list[i] = rand() % 10;
  }
  int sum;

  sum = sum_array(list, 10);
  printf("%d\n", sum);
}

int sum_array(int list[], int num_elements)
{
  int i, sum=0;
#pragma omp parallel for
  for (i=0; i<num_elements; i++)
  {
    if(list[i] == 3)
    sum = sum + list[i];
  }
  return(sum);
}


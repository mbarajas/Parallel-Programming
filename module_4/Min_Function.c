#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define __SSE__
#include "vVector.h"
#define N 20000000
int array_int_min(vInt32 a[], int n){
  int i;
  vInt32 min,temp,temp1;
  vCopy(min,a[0]);
//  printf("%d\n",vExtract_int(min,0));
  for(i=1; i < n - 1; i+=2){
    vMin_int(temp,a[i],a[i+1]);
//    vMin_int(temp1,a[i+2],a[i+3]);
    vMin_int(min,temp,min);
//    vMin_int(min,temp1,min);
//    printf("%d\n",vExtract_int(min,0));
  }
  vSplat_int(temp,min,0); vMin_int(min,temp,min);
  vSplat_int(temp,min,1); vMin_int(min,temp,min);
  vSplat_int(temp,min,2); vMin_int(min,temp,min);
  return vExtract_int(min,3);
}

int test[N];
int main(int argc, char *argv[]){
  int i,j;
  for(i = 0; i < N; i++){
    test[i] = rand() + 100;
    if(test[i] == 0)
      test[i] = 1337;
  // printf("%d\n",test[i]);
  }
  j = clock();
  i = array_int_min((vInt32*)test,N/4);
  printf("clock = %d min = %d\n", clock()-j,i);
  return 0;
}

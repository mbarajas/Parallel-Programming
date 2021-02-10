#include "omp.h"
#include <stdio.h>
#include <math.h>

int main (int argc, char *argv[]) {
  int numPrimes=0, numP41=0, numP43=0;   
  int i, j, limit;   
#pragma omp parallel for private(j,limit) \
  schedule(static, 100) \
  reduction(+ : numPrimes, numP41, numP43)   
  for(i = 3; i <= 1000000; i += 2) {   
    limit = (int) sqrt((float)i) + 1;      
    j = 3;   
    while (j <= limit && (i%j != 0))
      j+=2;          
    if (j>limit) {   
      numPrimes++;   
      if (i%4 == 1) numP41++;  // 4k+1 primes   
      if (i%4 == 3) numP43++;  // 4k-1 primes   
    }   //if j
  }   //for i
  printf("primes=%d p41=%d p43=%d\n",
      numPrimes, numP41, numP43);
  return 0;
}

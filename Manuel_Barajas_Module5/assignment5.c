#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "barrier.h" 

#define BITS 29

#define BARRIER_COUNT 1000
pthread_barrier_t barrier;

struct rs_args {
  int id;         
  unsigned *memVal;  
  unsigned *temp;  
  int n;          
  int *nzeros;    
  int *nones;     
  int t;  
};

struct rs_args *args;
void main (int argc, char *argv[]) 
{
  int num, tid;
  unsigned *memVal;
  int isSorted;
  int c;

    num = 1000000;

    tid = 5;
  memVal = (unsigned *) malloc (num * sizeof(unsigned));
   fflush (stdout);
  arrayRandomize (memVal, num);
  args = (struct rs_args *) malloc (tid * sizeof(struct rs_args));
  
  arrayPrint (memVal, num);

  printf ("Sorting array\n"); 
  fflush (stdout);
  c = clock();
  radix_sort (memVal, num, tid); 
  printf ("Done time: %d.\n", clock() - c);

  isSorted = arraySorted (memVal, num);
  if ( isSorted )
    printf ("Array sorted.\n");
  else
    printf ("Array not sorted.\n");
}
void arrayCopy (unsigned *dest, unsigned *first, int n)
{
  for ( ; n > 0; n-- )
    *dest++ = *first++;
}

void arrayPrint (unsigned *memVal, int n)
{
  int i;
  for ( i = 0; i < n; i++ )
    printf ("%d \n", memVal[i]);
  printf ("\n");
}

void arrayRandomize (unsigned *memVal, int n)
{
  int i;
  for ( i = 0; i < n; i++ ) {
  	memVal[i] = (unsigned)lrand48() & (unsigned)((1 << BITS) - 1);  	
  }
}

int arraySorted (unsigned *memVal, int n)
{
  int i;
  for ( i = 1; i < n; i++ )
    if ( memVal[i-1] > memVal[i] )
      return 0;
  return 1;
}
void radix_sort_thread (unsigned *memVal, 
			unsigned *temp,           
			int start, int n,        
			int *nzeros, int *nones, 
			int thread_index,        
			int t)                   
{

  unsigned *first, *dest;
  int bit_pos;
  int index0, index1;
  int i;
  first = memVal;
  dest = temp;
  for ( bit_pos = 0; bit_pos < BITS; bit_pos++ ) {

    nzeros[thread_index] = 0;
    for ( i = start; i < start + n; i++ ) {
      if ( ((first[i] >> bit_pos) & 1) == 0 ) {
	  	nzeros[thread_index]++;      	
      }	
    }
    nones[thread_index] = n - nzeros[thread_index];
	
    pthread_barrier_wait(&barrier);

    index0 = 0;
    index1 = 0;
    for ( i = 0; i < thread_index; i++ ) {
      index0 += nzeros[i];
      index1 += nones[i];
    }
    index1 += index0;
    for ( ; i < t; i++ ) {
      index1 += nzeros[i];
	}
	
    pthread_barrier_wait(&barrier);

    for ( i = start; i < start + n; i++ ) {
      if ( ((first[i] >> bit_pos) & 1) == 0 ) {
	  	dest[index0++] = first[i];      	
      } else {
	  	dest[index1++] = first[i];      	
      }
    }
	
    pthread_barrier_wait(&barrier);
	
    temp = first;
    first = dest;
    dest = temp;
  }
}

void thread_work (int rank)
{
  int start, count, n;
  int index = rank;
  n = args[index].n / args[index].t; 
  start = args[index].id * n; 
  radix_sort_thread (args[index].memVal, args[index].temp, start, n,
  		     args[index].nzeros, args[index].nones, args[index].id, args[index].t);
}
void radix_sort (unsigned *memVal, int n, int t)
{
  unsigned *temp;
  int *nzeros, *nones;
  int r, i;
  
  long thread;
  pthread_t* thread_handles;

  temp = (unsigned *) malloc (n * sizeof(unsigned));
  if (!temp) { fprintf (stderr, "Malloc failed.\n"); exit(1); }

  nzeros = (int *) malloc (t * sizeof(int));
  if (!nzeros) { fprintf (stderr, "Malloc failed.\n"); exit(1); }
  nones = (int *) malloc (t * sizeof(int));
  if (!nones) { fprintf (stderr, "Malloc failed.\n"); exit(1); }

  thread_handles = malloc (t * sizeof(pthread_t));
  pthread_barrier_init (&barrier, NULL, t);
  
  for ( i = 0; i < t; i++ ) {
    args[i].id = i;
    args[i].memVal = memVal;
    args[i].temp = temp;
    args[i].n = n;
    args[i].nzeros = nzeros;
    args[i].nones = nones;
    args[i].t = t;
	
    pthread_create (&thread_handles[i], NULL, thread_work, i);
  }
  
  for ( i = 0; i < t; i++ ) {
    pthread_join (thread_handles[i], NULL);
  }

  pthread_barrier_destroy(&barrier);
  free (thread_handles);
  free (args);

  arrayPrint (memVal, n);
  arrayPrint (temp, n);

  if ( BITS % 2 == 1 ) {
    arrayCopy (memVal, temp, n);  	
  }

  free (nzeros);
  free (nones);
  free (temp);
}





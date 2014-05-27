#include <pthread.h>

#define THREADS 2
#define ARRAYSIZE 6
#define ITERATIONS   ARRAYSIZE / THREADS

int  sum=0;
int numArray[ARRAYSIZE];
pthread_mutex_t sum_mutex;

void *thread_sum(void *tid)
{
  int start;
  int *mytid;
  int end;
  int mysum=0;

  mytid = (int *) tid;
  start = (*mytid * ITERATIONS);
  end = start + ITERATIONS;

  int i;
  for (i=start; i < end ; i++) {
    mysum = mysum + numArray[i];
  }

  pthread_mutex_lock (&sum_mutex);
  sum = sum + mysum;
  pthread_mutex_unlock (&sum_mutex);
  pthread_exit(NULL);
}

void main(int argc, char *argv[])
{
  int i;
  for(i=0; i<argc; i++){
    numArray[i] = atoi(argv[i]);
  }

  pthread_t threads[THREADS];
  pthread_attr_t attr;

  pthread_mutex_init(&sum_mutex, NULL);
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  int tids[THREADS];
  for (i=0; i<THREADS; i++) {
    tids[i] = i;
    pthread_create(&threads[i], &attr, thread_sum, (void *) &tids[i]);
  }

  for (i=0; i<THREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  printf ("%d", sum);

  pthread_attr_destroy(&attr);
  pthread_mutex_destroy(&sum_mutex);
  pthread_exit (NULL);
}

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int list[10];
pthread_mutex_t mVar;
int sum = 0;
void *routine(void *ignored)
{
  int local_index, partial_sum = 0;
  for(local_index = 0; local_index < 10; local_index++){

      if(*(list + local_index) == 3 )
      partial_sum += *(list + local_index);
  }
  pthread_mutex_lock(&mVar);
  sum += partial_sum;
  pthread_mutex_unlock(&mVar);

}

int main()
{
  pthread_t tid[2];
  pthread_mutex_init(&mVar, NULL);
  int i = 0;
  for (i = 0; i < 10; i++)
    list[i] = rand () % 10;

  for (i = 0; i < 2; i++){
    pthread_create(&tid[i], NULL, routine, NULL);
    pthread_join(tid[i], NULL);
  }
  printf("%d\n", sum);
}

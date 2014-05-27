#include <stdio.h>
#include <pthread.h>
#include <assert.h>

typedef struct {
  int *row;
  int *column;
  int *result;
  int n, columnStride;
} Work;

void *p(void *arg) {
  Work *w=(Work *)arg;
  int i;
  *w->result = 0;
  for (i=0; i<w->n; i++) {
    *w->result += w->row[i]*
    w->column[i*w->columnStride];
  }
  printf("%d\n", *w->result);
  pthread_exit((void *)0);
}

//result = 3x2 * 2x3
int left[3][2]={{1,2},{3,4},{5,6}};
int right[2][3]={{1,2,3},{4,5,6}};
int result[3][3];

int main() {
  pthread_t x[9];
  Work args[9];
  void *r;
  int i,j;
  for (i=0; i<3; i++)
  for (j=0; j<3; j++) {
    args[i*3+j].row=&left[i][0];
    args[i*3+j].column=&right[0][j];
    args[i*3+j].result=&result[i][j];
    args[i*3+j].n=2;
    args[i*3+j].columnStride=3;
    assert(
    pthread_create(&x[i*3+j], NULL, p,
    (void *)&args[i*3+j]) == 0);
  } //for j
  for (i=0; i<9; i++) {
    assert(pthread_join(x[i], &r) == 0);
  }
  for (i=0; i<3; i++)
  for (j=0; j<3; j++)
  printf("%d ", result[i][j]);
  return 0;
}

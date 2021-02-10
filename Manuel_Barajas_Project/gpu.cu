#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 8192
#define MEMSIZE 32


__global__ void multiplyTest(double * one, double * two, double * three,
                                     int size) {
    __shared__ double s_first[MEMSIZE][MEMSIZE];
    __shared__ double s_second[MEMSIZE][MEMSIZE];

    int bx = blockIdx.x;
    int by = blockIdx.y;
    int tx = threadIdx.x;
    int ty = threadIdx.y;

    int row = by * MEMSIZE + ty;
    int col = bx * MEMSIZE + tx;

    double elementSum = 0;

    for(int m = 0; m < size / MEMSIZE; m++) {
        s_first[ty][tx] = one[row * size + m * MEMSIZE + tx];
        s_second[ty][tx] = two[(m * MEMSIZE + ty) * size + col];

        __syncthreads();

        for(int k = 0; k < MEMSIZE; k++)
            elementSum += s_first[ty][k] * s_second[k][tx];

        __syncthreads();

    }
    three[row * size + col] = elementSum;
}

int main() {
    int c;
    double *h_first = (double *) malloc(SIZE * SIZE * sizeof(double));
    double *h_second = (double *) malloc(SIZE * SIZE * sizeof(double));
    double *h_sum = (double *) malloc(SIZE * SIZE * sizeof(double));

    long i;
    for(i = 0; i < SIZE * SIZE; i++) {
        h_first[i] = 2.0;
        h_second[i] = 2.0;
        h_sum[i] = 0.0;
    }

    double *d_first = NULL;
    double *d_second = NULL;
    double *d_sum = NULL;

    cudaMalloc((void **) &d_first, SIZE * SIZE * sizeof(double));
    cudaMalloc((void **) &d_second, SIZE * SIZE * sizeof(double));
    cudaMalloc((void **) &d_sum, SIZE * SIZE * sizeof(double));

    cudaMemcpy(d_first, h_first, SIZE * SIZE * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(d_second, h_second, SIZE * SIZE * sizeof(double), cudaMemcpyHostToDevice);

    dim3 dimGrid(SIZE / MEMSIZE, SIZE / MEMSIZE,1);
    dim3 dimBlock(MEMSIZE, MEMSIZE, 1);

    c = clock();

    multiplyTest<<<dimGrid,dimBlock>>>(d_first, d_second, d_sum, SIZE);

    cudaThreadSynchronize();
    cudaMemcpy(h_sum, d_sum, SIZE * SIZE * sizeof(double), cudaMemcpyDeviceToHost);

    printf("Done. Time: %d\n", clock()-c);

    return 0;
}



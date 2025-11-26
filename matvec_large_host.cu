#include <assert.h>
#include <stdio.h>
#include "matvec_large_kernel.hu"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 4096    // large matrix size
#define LOOPS 1000  // number of iterations

void matvec(float A[N][N], float x[N], float y[N]) {
    {
#define cudaCheckReturn(ret) \
  do { \
    cudaError_t cudaCheckReturn_e = (ret); \
    if (cudaCheckReturn_e != cudaSuccess) { \
      fprintf(stderr, "CUDA error: %s\n", cudaGetErrorString(cudaCheckReturn_e)); \
      fflush(stderr); \
    } \
    assert(cudaCheckReturn_e == cudaSuccess); \
  } while(0)
#define cudaCheckKernel() \
  do { \
    cudaCheckReturn(cudaGetLastError()); \
  } while(0)

      float *dev_A;
      float *dev_x;
      float *dev_y;
      
      cudaCheckReturn(cudaMalloc((void **) &dev_A, (4096) * (4096) * sizeof(float)));
      cudaCheckReturn(cudaMalloc((void **) &dev_x, (4096) * sizeof(float)));
      cudaCheckReturn(cudaMalloc((void **) &dev_y, (4096) * sizeof(float)));
      
      cudaCheckReturn(cudaMemcpy(dev_A, A, (4096) * (4096) * sizeof(float), cudaMemcpyHostToDevice));
      cudaCheckReturn(cudaMemcpy(dev_x, x, (4096) * sizeof(float), cudaMemcpyHostToDevice));
      cudaCheckReturn(cudaMemcpy(dev_y, y, (4096) * sizeof(float), cudaMemcpyHostToDevice));
      {
        dim3 k0_dimBlock(32);
        dim3 k0_dimGrid(128);
        kernel0 <<<k0_dimGrid, k0_dimBlock>>> (dev_A, dev_x, dev_y);
        cudaCheckKernel();
      }
      
      cudaCheckReturn(cudaMemcpy(y, dev_y, (4096) * sizeof(float), cudaMemcpyDeviceToHost));
      cudaCheckReturn(cudaFree(dev_A));
      cudaCheckReturn(cudaFree(dev_x));
      cudaCheckReturn(cudaFree(dev_y));
    }
}


int main() {
    clock_t start = clock();
    // use static arrays instead of malloc
    static float A[N][N];
    static float x[N];
    static float y[N];

    // initialize with random values
    srand(time(NULL));
    for (int i = 0; i < N; i++) {
        x[i] = (float)(rand() % 100) / 10.0f;
        for (int j = 0; j < N; j++) {
            A[i][j] = (float)(rand() % 100) / 10.0f;
        }
    }

    // run multiple times to monitor GPU usage
    for (int k = 0; k < LOOPS; k++) {
        printf("Iteration %d\n", k+1);
        matvec(A, x, y);
    }

    // print first 10 results of last iteration
    // printf("y[0..9] = ");
    // for (int i = 0; i < 10; i++) printf("%f ", y[i]);
    // printf("\n");

    clock_t end = clock();    // record end time
    double total_time = (double)(end - start) / CLOCKS_PER_SEC; // in seconds

    printf("Total time: %f seconds\n", total_time);
    return 0;
}


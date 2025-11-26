#include <assert.h>
#include <stdio.h>
#include "ocl_utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 4096    // large matrix size
#define LOOPS 1000  // number of iterations

void matvec(float A[N][N], float x[N], float y[N]) {
    {
      #define openclCheckReturn(ret) \
  if (ret != CL_SUCCESS) {\
    fprintf(stderr, "OpenCL error: %s\n", opencl_error_string(ret)); \
    fflush(stderr); \
    assert(ret == CL_SUCCESS);\
  }

      cl_mem dev_A;
      cl_mem dev_x;
      cl_mem dev_y;
      
      cl_device_id device;
      cl_context context;
      cl_program program;
      cl_command_queue queue;
      cl_int err;
      device = opencl_create_device(1);
      context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
      openclCheckReturn(err);
      queue = clCreateCommandQueue(context, device, 0, &err);
      openclCheckReturn(err);
      program = opencl_build_program_from_file(context, device, "matvec_large_kernel.cl", "");
      
      {
        dev_A = clCreateBuffer(context, CL_MEM_READ_WRITE, (4096) * (4096) * sizeof(float), NULL, &err);
        openclCheckReturn(err);
      }
      {
        dev_x = clCreateBuffer(context, CL_MEM_READ_WRITE, (4096) * sizeof(float), NULL, &err);
        openclCheckReturn(err);
      }
      {
        dev_y = clCreateBuffer(context, CL_MEM_READ_WRITE, (4096) * sizeof(float), NULL, &err);
        openclCheckReturn(err);
      }
      
      openclCheckReturn(clEnqueueWriteBuffer(queue, dev_A, CL_TRUE, 0, (4096) * (4096) * sizeof(float), A, 0, NULL, NULL));
      openclCheckReturn(clEnqueueWriteBuffer(queue, dev_x, CL_TRUE, 0, (4096) * sizeof(float), x, 0, NULL, NULL));
      openclCheckReturn(clEnqueueWriteBuffer(queue, dev_y, CL_TRUE, 0, (4096) * sizeof(float), y, 0, NULL, NULL));
      {
        size_t global_work_size[1] = {(128) * 32};
        size_t block_size[1] = {32};
        cl_kernel kernel0 = clCreateKernel(program, "kernel0", &err);
        openclCheckReturn(err);
        openclCheckReturn(clSetKernelArg(kernel0, 0, sizeof(cl_mem), (void *) &dev_A));
        openclCheckReturn(clSetKernelArg(kernel0, 1, sizeof(cl_mem), (void *) &dev_x));
        openclCheckReturn(clSetKernelArg(kernel0, 2, sizeof(cl_mem), (void *) &dev_y));
        openclCheckReturn(clEnqueueNDRangeKernel(queue, kernel0, 1, NULL, global_work_size, block_size, 0, NULL, NULL));
        openclCheckReturn(clReleaseKernel(kernel0));
        clFinish(queue);
      }
      
      openclCheckReturn(clEnqueueReadBuffer(queue, dev_y, CL_TRUE, 0, (4096) * sizeof(float), y, 0, NULL, NULL));
      openclCheckReturn(clReleaseMemObject(dev_A));
      openclCheckReturn(clReleaseMemObject(dev_x));
      openclCheckReturn(clReleaseMemObject(dev_y));
      openclCheckReturn(clReleaseCommandQueue(queue));
      openclCheckReturn(clReleaseProgram(program));
      openclCheckReturn(clReleaseContext(context));
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


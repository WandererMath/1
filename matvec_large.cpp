#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define N 4096    // large matrix size
#define LOOPS 1000  // number of iterations

void matvec(float A[N][N], float x[N], float y[N]) {
    #pragma omp parallel for
    #pragma scop
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            y[i] += A[i][j] * x[j];
    #pragma endscop
}


int main() {
    double start = omp_get_wtime();
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

    // clock_t end = clock();    // record end time
    // double total_time = (double)(end - start) / CLOCKS_PER_SEC; // in seconds

    // printf("Total time: %f seconds\n", total_time);

    double end = omp_get_wtime();
    printf("Time = %f seconds\n", end - start);
    return 0;
}


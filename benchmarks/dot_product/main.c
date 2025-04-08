#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define N 100000000  // 100 million elements

int main() {
    float *A = malloc(N * sizeof(float));
    float *B = malloc(N * sizeof(float));
    float sum = 0.0;

    // Seed RNG and fill arrays
    srand(time(NULL));
    for (int i = 0; i < N; ++i) {
        A[i] = (float)rand() / RAND_MAX;
        B[i] = (float)rand() / RAND_MAX;
    }

    double start = omp_get_wtime();

    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < N; ++i) {
        sum += A[i] * B[i];
    }

    double end = omp_get_wtime();
    printf("Dot product = %f\n", sum);
    printf("Time elapsed: %f seconds\n", end - start);

    free(A);
    free(B);
    return 0;
}

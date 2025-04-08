#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <math.h>

#define N 1000
#define K 1000
#define M 1000

int main() {
    float *A = malloc(N * K * sizeof(float));
    float *B = malloc(K * M * sizeof(float));
    float *C = malloc(N * M * sizeof(float));

    srand(time(NULL));
    for (int i = 0; i < N * K; ++i) A[i] = (float)rand() / RAND_MAX;
    for (int i = 0; i < K * M; ++i) B[i] = (float)rand() / RAND_MAX;

    double start = omp_get_wtime();

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            float sum = 0.0f;
            for (int k = 0; k < K; ++k) {
                sum += A[i * K + k] * B[k * M + j];
            }
            C[i * M + j] = sum;
        }
    }

    double end = omp_get_wtime();
    printf("Matrix Multiplication Completed\n");
    printf("Time elapsed: %f seconds\n", end - start);

    // --- Sequential version for correctness check ---
    float *C_seq = malloc(N * M * sizeof(float));

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            float sum = 0.0f;
            for (int k = 0; k < K; ++k) {
                sum += A[i * K + k] * B[k * M + j];
            }
            C_seq[i * M + j] = sum;
        }
    }

    // Compare element-by-element
    int correct = 1;
    for (int i = 0; i < N * M; ++i) {
        if (fabs(C[i] - C_seq[i]) > 1e-3) {  // adjust tolerance if needed
            printf("Mismatch at index %d: %f vs %f\n", i, C[i], C_seq[i]);
            correct = 0;
            break;
        }
    }

    if (correct) {
        printf("Matrix multiplication correct!\n");
    } else {
        printf("Incorrect matrix multiplication result.\n");
    }

    free(C_seq);

    free(A);
    free(B);
    free(C);
    return 0;

}


#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <math.h>

#define R 8000
#define C 8000

int main() {
    float *A = malloc(R * C * sizeof(float));
    float *res = malloc(R * C * sizeof(float));
    float *res_seq = malloc(R * C * sizeof(float));

    srand(time(NULL));
    for (int i = 0; i < R * C; ++i) A[i] = (float)rand() / RAND_MAX;

    // === Zero out edges ===
    #pragma omp parallel for
    for (int i = 0; i < R; ++i) {
        res[i * C + 0] = 0.0;
        res[i * C + (C - 1)] = 0.0;
    }

    #pragma omp parallel for
    for (int j = 0; j < C; ++j) {
        res[0 * C + j] = 0.0;
        res[(R - 1) * C + j] = 0.0;
    }

    // === Parallel Blur ===
    double start = omp_get_wtime();

    #pragma omp parallel for collapse(2)
    for (int i = 1; i < R - 1; ++i) {
        for (int j = 1; j < C - 1; ++j) {
            float sum = 0.0;
            for (int di = -1; di <= 1; ++di) {
                for (int dj = -1; dj <= 1; ++dj) {
                    sum += A[(i + di) * C + (j + dj)];
                }
            }
            res[i * C + j] = sum / 9.0;
        }
    }

    double end = omp_get_wtime();
    printf("Image Blur Completed\n");
    printf("Time elapsed: %f seconds\n", end - start);

    // === Sequential Blur (for correctness check) ===
    for (int i = 0; i < R * C; ++i) res_seq[i] = 0.0;

    for (int i = 1; i < R - 1; ++i) {
        for (int j = 1; j < C - 1; ++j) {
            float sum = 0.0;
            for (int di = -1; di <= 1; ++di) {
                for (int dj = -1; dj <= 1; ++dj) {
                    sum += A[(i + di) * C + (j + dj)];
                }
            }
            res_seq[i * C + j] = sum / 9.0;
        }
    }

    // === Compare ===
    int correct = 1;
    for (int i = 1; i < R - 1 && correct; ++i) {
        for (int j = 1; j < C - 1; ++j) {
            float diff = fabs(res[i * C + j] - res_seq[i * C + j]);
            if (diff > 1e-5) {
                correct = 0;
                printf("Mismatch at (%d, %d): parallel=%.6f, serial=%.6f\n",
                       i, j, res[i * C + j], res_seq[i * C + j]);
                break;
            }
        }
    }

    if (correct) {
        printf("✅ Image Blur correct!\n");
    } else {
        printf("❌ Incorrect Image Blur result.\n");
    }

    free(res_seq);
    free(res);
    free(A);
    return 0;
}



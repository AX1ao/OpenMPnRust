#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define N 100000000

int main() {
    double *A = malloc(N * sizeof(double));
    double *S = malloc(N * sizeof(double));
    double *block_totals = malloc(64 * sizeof(double));
    int nthreads;

    srand(time(NULL));
    for (int i = 0; i < N; ++i) A[i] = 1.0f;

    double start = omp_get_wtime();

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();

        #pragma omp single
        {
            nthreads = omp_get_num_threads();
        }

        int chunk = (N + nthreads - 1) / nthreads;  // ceiling division
        int start_i = tid * chunk;
        int end_i = (start_i + chunk > N) ? N : start_i + chunk;


        // Step 1: local prefix sum
        S[start_i] = A[start_i];
        for (int i = start_i + 1; i < end_i; ++i) {
            S[i] = S[i - 1] + A[i];
        }

        // Save block total
        block_totals[tid] = S[end_i - 1];

        #pragma omp barrier

        // Step 2: sequential scan on block totals (done by one thread)
        #pragma omp single
        {
            for (int i = 1; i < nthreads; ++i) {
                block_totals[i] += block_totals[i - 1];
            }
        }

        #pragma omp barrier

        // Step 3: offset adjust (except thread 0)
        if (tid != 0) {
            float offset = block_totals[tid - 1];
            for (int i = start_i; i < end_i; ++i) {
                S[i] += offset;
            }
        }
    }

    double end = omp_get_wtime();
    printf("Prefix Sum Completed\n");
    printf("Time elapsed: %f seconds\n", end - start);

    // Check correctness
    if ((int)S[N - 1] == N) {
        printf(" 🙌🏼 Prefix sum correct!\n");
    } else {
        printf(" 🙅🏼‍♀️ Incorrect result: S[N-1] = %f (expected %d)\n", S[N - 1], N);
    }

    free(A);
    free(S);
    free(block_totals);
    return 0;
}



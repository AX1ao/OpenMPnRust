#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define N 100000000  // size of array to sort

// Comparison function for qsort (ascending float)
int compare_floats(const void *a, const void *b) {
    float fa = *(const float*)a;
    float fb = *(const float*)b;
    return (fa > fb) - (fa < fb); // 1, 0, -1
}

// Merges two sorted chunks into output array
void merge(float *A, int start1, int end1, int start2, int end2, float *output, int output_start) {
    int i = start1, j = start2, k = output_start;
    while (i <= end1 && j <= end2) {
        output[k++] = (A[i] <= A[j]) ? A[i++] : A[j++];
    }
    while (i <= end1) output[k++] = A[i++];
    while (j <= end2) output[k++] = A[j++];
}

int main() {
    float *A = malloc(N * sizeof(float));
    float *B = malloc(N * sizeof(float));  // buffer for merging
    int nthreads;

    // Fill A with random floats
    srand(time(NULL));
    for (int i = 0; i < N; ++i) {
        A[i] = (float)rand() / RAND_MAX;
    }

    double start = omp_get_wtime();

    // Step 1: Parallel sort chunks
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        #pragma omp single
        {
            nthreads = omp_get_num_threads();
        }

        int chunk_size = (N + nthreads - 1) / nthreads;
        int start_i = tid * chunk_size;
        int end_i = (start_i + chunk_size > N) ? N : start_i + chunk_size;

        qsort(&A[start_i], end_i - start_i, sizeof(float), compare_floats);
    }

    // Step 2: Merge chunks pairwise (log(nthreads) levels)
    int step = 1;
    int toggle = 0;  // switch between A and B buffers
    while (step < nthreads) {
        int merged_chunks = 0;
        #pragma omp parallel for
        for (int i = 0; i < nthreads; i += 2 * step) {
            int start1 = i * (N / nthreads);
            int mid1 = start1 + step * (N / nthreads) - 1;
            int start2 = mid1 + 1;
            int end2 = start2 + step * (N / nthreads) - 1;

            if (start2 >= N) continue;
            if (end2 >= N) end2 = N - 1;
            if (mid1 >= N) mid1 = N - 1;

            float *src = toggle ? B : A;
            float *dst = toggle ? A : B;

            merge(src, start1, mid1, start2, end2, dst, start1);
        }

        toggle = 1 - toggle;
        step *= 2;
    }

    double end = omp_get_wtime();
    printf("Quicksort (chunked + merged) Completed\n");
    printf("Time elapsed: %f seconds\n", end - start);

    // Step 3: Check correctness
    float *final = (toggle ? B : A);
    int correct = 1;
    for (int i = 1; i < N; ++i) {
        if (final[i] < final[i - 1]) {
            printf("❌ Not sorted at index %d: %.6f < %.6f\n", i, final[i], final[i - 1]);
            correct = 0;
            break;
        }
    }
    if (correct) printf("✅ Quicksort result correct!\n");

    free(A);
    free(B);
    return 0;
}

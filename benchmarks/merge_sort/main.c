#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define N 100000000
#define MIN_CHUNK_SIZE 100000  // stop spawning tasks below this size

// Merge two sorted halves: A[l..m], A[m+1..r] into temp[]
void merge(float *A, float *temp, int l, int m, int r) {
    int i = l, j = m + 1, k = l;

    while (i <= m && j <= r) {
        temp[k++] = (A[i] <= A[j]) ? A[i++] : A[j++];
    }
    while (i <= m) temp[k++] = A[i++];
    while (j <= r) temp[k++] = A[j++];

    for (int x = l; x <= r; ++x) A[x] = temp[x];
}

// Recursive parallel merge sort
void merge_sort(float *A, float *temp, int l, int r) {
    if (l >= r) return;

    int m = (l + r) / 2;

    // Parallel recursion if large enough
    if (r - l > MIN_CHUNK_SIZE) {
        #pragma omp task shared(A, temp)
        merge_sort(A, temp, l, m);
        #pragma omp task shared(A, temp)
        merge_sort(A, temp, m + 1, r);
        #pragma omp taskwait
    } else {
        merge_sort(A, temp, l, m);
        merge_sort(A, temp, m + 1, r);
    }

    merge(A, temp, l, m, r);
}

int main() {
    float *A = malloc(N * sizeof(float));
    float *temp = malloc(N * sizeof(float));

    srand(time(NULL));
    for (int i = 0; i < N; ++i) {
        A[i] = (float)rand() / RAND_MAX;
    }

    double start = omp_get_wtime();

    #pragma omp parallel
    {
        #pragma omp single
        merge_sort(A, temp, 0, N - 1);
    }

    double end = omp_get_wtime();
    printf("Merge Sort Completed\n");
    printf("Time elapsed: %f seconds\n", end - start);

    // Check correctness (simplified check)
    int correct = 1;
    for (int i = 1; i < N; ++i) {
        if (A[i] < A[i - 1]) {
            printf("❌ Not sorted at index %d: %.6f < %.6f\n", i, A[i], A[i - 1]);
            correct = 0;
            break;
        }
    }
    if (correct) printf("✅ Merge sort result correct!\n");

    free(A);
    free(temp);
    return 0;
}

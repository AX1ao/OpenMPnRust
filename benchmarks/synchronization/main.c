#include <omp.h>
#include <stdio.h>

#define INCREMENTS_PER_THREAD 100000

int main() {
    long long counter = 0;
    int num_threads = 0;

    double start = omp_get_wtime();

    #pragma omp parallel
    {
        #pragma omp atomic
        num_threads++;

        for (int i = 0; i < INCREMENTS_PER_THREAD; i++) {
            #pragma omp atomic
            counter++;
        }
    }

    double end = omp_get_wtime();
    double elapsed_ms = (end - start) * 1000.0;

    printf("Time elapsed = %.3f ms\n", elapsed_ms);
    printf("Result = %lld\n", counter);

    return 0;
}

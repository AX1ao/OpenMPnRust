#include <omp.h>
#include <stdio.h>

#define REPEATS 100

int main() {
    int total_threads = 0;
    double start = omp_get_wtime();

    for (int r = 0; r < REPEATS; r++) {
        int thread_counter = 0;

        #pragma omp parallel
        {
            #pragma omp atomic
            thread_counter++;
        }

        total_threads = thread_counter; // should be the same every time
    }

    double end = omp_get_wtime();
    double avg_time_ms = ((end - start) * 1000.0) / REPEATS;

    printf("Time elapsed = %.3f ms\n", avg_time_ms);
    printf("Result = %d\n", total_threads);
    return 0;
}

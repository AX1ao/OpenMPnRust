#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define N 100000000

int main() {
    int count = 0;

    double start = omp_get_wtime();

    #pragma omp parallel reduction(+:count)
    {
        int tid = omp_get_thread_num();
        int nthreads = omp_get_num_threads();
        int chunk = (N + nthreads - 1) / nthreads;
        int start_i = tid * chunk;
        int end_i = (start_i + chunk > N) ? N : start_i + chunk;

        unsigned int seed = time(NULL) + tid;

        for (int i = start_i; i < end_i; ++i) {
            double x = (double)rand_r(&seed) / RAND_MAX;
            double y = (double)rand_r(&seed) / RAND_MAX;
            if (x * x + y * y <= 1.0) {
                count++;
            }
        }
    }

    double end = omp_get_wtime();
    printf("Pi Estimation Completed\n");
    printf("Time elapsed: %f seconds\n", end - start);

    double pi_est = 4.0 * (double)count / N;
    printf("The Pi_est is %.8f\n", pi_est);

    return 0;
}

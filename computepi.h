#include <stdio.h>

typedef struct __PI_ITEM {
    int from, to;
    int N;
    double *result;
} pitem;

double compute_pi_baseline(size_t N);
double compute_pi_pthread(size_t N, int NUM_THREADS);
void *piThread(void *pitem_array);
double compute_pi_openmp(size_t N, int threads);
double compute_pi_avx(size_t N);
double compute_pi_avx_unroll(size_t N);

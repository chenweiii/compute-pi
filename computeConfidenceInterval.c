#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "computepi.h"

#define SAMPLES 50
#define ONE_SEC 1000000000.0
#define CLOCK_ID CLOCK_MONOTONIC_RAW

double computeAVG(double *data, int num)
{
    double sum = 0.0;
    for (int i = 0; i < num; i++)
        sum += data[i];
    return sum / num;
}

double computeSTD(double *data)
{
    double sum = 0.0;
    double avg = computeAVG(data, SAMPLES);
    for (int i = 0; i < SAMPLES; i++)
        sum += pow(data[i] - avg, 2);
    return sqrt(sum / (double)(SAMPLES - 1));
}

/* return the valid sample number */
int filterData(double *data)
{
    double avg = computeAVG(data, SAMPLES);
    double std = computeSTD(data);
    double ceiling = avg + 2 * std;
    double floor = avg - 2 * std;
    int num = 0;

    for (int i = 0; i < SAMPLES; i++) {
        if (data[i] <= ceiling && data[i] >= floor)
            num++;
        else
            data[i] = 0;
    }

    return num;
}

int main(int argc, char const *argv[])
{
    struct timespec start = {0, 0};
    struct timespec end = {0, 0};

    if (argc < 2) return -1;

    int N = atoi(argv[1]);
    int loop = 25;

    double BASELINE[SAMPLES], PTHREAD_2[SAMPLES], PTHREAD_4[SAMPLES],
           PTHREAD_8[SAMPLES], PTHREAD_16[SAMPLES];
    // Valid Samples number (between 95% confidence interval
    int vnum_BASELINE, vnum_PTHREAD_2, vnum_PTHREAD_4, vnum_PTHREAD_8,
        vnum_PTHREAD_16;

    // Baseline
    for (int i = 0; i < SAMPLES; i++) {
        clock_gettime(CLOCK_ID, &start);
        for (int j = 0; j < loop; j++) {
            compute_pi_baseline(N);
        }
        clock_gettime(CLOCK_ID, &end);
        BASELINE[i] = (double) (end.tv_sec - start.tv_sec) +
                      (end.tv_nsec - start.tv_nsec)/ONE_SEC;
    }

    // Pthread with 2 threads
    for (int i = 0; i < SAMPLES; i++) {
        clock_gettime(CLOCK_ID, &start);
        for (int j = 0; j < loop; j++) {
            compute_pi_pthread(N, 2);
        }
        clock_gettime(CLOCK_ID, &end);
        PTHREAD_2[i] = (double) (end.tv_sec - start.tv_sec) +
                       (end.tv_nsec - start.tv_nsec)/ONE_SEC;
    }

    vnum_BASELINE = filterData(BASELINE);
    vnum_PTHREAD_2 = filterData(PTHREAD_2);

    FILE *fout = fopen("output.txt", "a+");
    fprintf(fout, "%d %f %f\n", N, computeAVG(BASELINE, vnum_BASELINE),
            computeAVG(PTHREAD_2, vnum_PTHREAD_2));
    fclose(fout);

    return 0;
}

CC = gcc
CFLAGS = -O0 -std=gnu99 -Wall -fopenmp
EXECUTABLE = \
	time_test_baseline time_test_openmp_2 time_test_openmp_4 \
	time_test_avx time_test_avxunroll time_test_pthread_2 \
	benchmark_clock_gettime computeConfidenceInterval

GIT_HOOKS := .git/hooks/pre-commit

$(GIT_HOOKS):
	@scripts/install-git-hooks
	@echo

default: $(GIT_HOOKS) computepi.o
	$(CC) $(CFLAGS) computepi.o time_test.c -DBASELINE -o time_test_baseline
	$(CC) $(CFLAGS) computepi.o time_test.c -DPTHREAD_2 -o time_test_pthread_2
#	$(CC) $(CFLAGS) computepi.o time_test.c -DOPENMP_2 -o time_test_openmp_2
#	$(CC) $(CFLAGS) computepi.o time_test.c -DOPENMP_4 -o time_test_openmp_4
#	$(CC) $(CFLAGS) computepi.o time_test.c -DAVX -o time_test_avx
#	$(CC) $(CFLAGS) computepi.o time_test.c -DAVXUNROLL -o time_test_avxunroll
	$(CC) $(CFLAGS) computepi.o benchmark_clock_gettime.c -o benchmark_clock_gettime
	$(CC) -O0 -g -std=gnu99 -Wall -pthread computepi.c computeConfidenceInterval.c -o computeConfidenceInterval -lm
.PHONY: clean default

computeConfidenceInterval:
	$(CC) -O0 -g -std=gnu99 -Wall -pthread computepi.c computeConfidenceInterval.c -o computeConfidenceInterval -lm

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

check: default
	time ./time_test_baseline
	time ./time_test_pthread_2
#	time ./time_test_openmp_2
#	time ./time_test_openmp_4
#	time ./time_test_avx
#	time ./time_test_avxunroll

gencsv: default
	for i in `seq 100 5000 25000`; do \
		printf "%d," $$i;\
		./benchmark_clock_gettime $$i; \
	done > result_clock_gettime.csv

plot: computeConfidenceInterval
	for i in `seq 100 1000 25000`; do \
		./computeConfidenceInterval $$i; \
	done
	gnuplot scripts/draw.gp

clean:
	rm -f $(EXECUTABLE) *.o *.s result_clock_gettime.csv *.txt *.png

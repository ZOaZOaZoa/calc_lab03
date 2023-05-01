#ifndef INT_FUNCTIONS_H_INCLUDED
#define INT_FUNCTIONS_H_INCLUDED

#include <vector>
#include <omp.h>
#include <math.h>

double sum_interval_parallel(double f(double), double start, double end, double step, int thread_count);
double simpson_parallel(double f(double), double a, double b, double h, int thread_count);
double simpson_parallel_runge(double f(double), double a, double b, double eps, int thread_count);

#endif //INT_FUNCTIONS_H_INCLUDED
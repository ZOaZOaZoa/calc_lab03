#ifndef INT_FUNCTIONS_H_INCLUDED
#define INT_FUNCTIONS_H_INCLUDED

#include <vector>
#include <omp.h>
#include <math.h>


double f(double x);
double f_prime(double x);
double sum_interval_parallel(double start, double end, double step, int thread_count);
double simpson_parallel(double f(double), double a, double b, double h, int thread_count);

#endif //INT_FUNCTIONS_H_INCLUDED
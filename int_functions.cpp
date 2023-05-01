#include "int_functions.h"

double f(double x)
{
    return (2*x + 3)*pow(4, 2*x);
}

double f_prime(double x)
{
    return 2*pow(4, 2*x)*(1+log(4)*(2*x+3));
}

double sum_interval_parallel(double start, double end, double step, int thread_count)
{
    omp_set_num_threads(thread_count);
    std::vector<double> thread_sum(thread_count);
    size_t steps = std::round((end-start)/step);
    #pragma omp parallel
    {
        int ID = omp_get_thread_num();
        thread_sum[ID] = 0;
        #pragma omp for
        for(size_t i = 0; i <= steps; i++)
        {
            thread_sum[ID] += f(start + i*step);
        }
    }

    double sum = 0;
    for (int i = 0; i < thread_count; i++)
    {
        sum += thread_sum[i];
    }

    return sum;
}

double simpson_parallel(double f(double), double a, double b, double h, int thread_count)
{
    double sum_x = sum_interval_parallel(a + h, b - h, h, thread_count);

    double sum_halfX = sum_interval_parallel(a + h/2, b - h/2, h, thread_count);

    return h/6*(f(a) + 4*sum_halfX + 2*sum_x + f(b));
}
#include "int_functions.h"

double sum_interval_parallel(double f(double), double start, double end, double step, int thread_count)
{
    size_t steps = std::round((end-start)/step);
    
    omp_set_num_threads(thread_count);
    std::vector<double> thread_sum(thread_count);
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
    double sum_x = sum_interval_parallel(f, a + h, b - h, h, thread_count);
    double sum_halfX = sum_interval_parallel(f, a + h/2, b - h/2, h, thread_count);
    return h/6*(f(a) + 4*sum_halfX + 2*sum_x + f(b));
}

double simpson_parallel_runge(double f(double), double a, double b, double eps, int thread_count, double& h_min)
{
    const int p = 4;
    const int MAX_ITER = 25;
    
    size_t it = 1;
    double h = 0.1;
    double I_h = simpson_parallel(f, a, b, h, thread_count);
    double I_2h, h2, rung;
    do
    {
        h2 = h;
        h = h2/2;
        I_2h = I_h;
        I_h = simpson_parallel(f, a, b, h, thread_count);
        rung = (I_h-I_2h)/(pow(2, p) - 1);
    } while (it++ < MAX_ITER & fabs(rung) > eps);

    if(it >= MAX_ITER)
    {
        I_h += rung;
    }

    h_min = h;

    return I_h;
}
#include <iostream>
#include <iomanip>
#include <numeric>
#include <chrono>

#include "int_functions.h"

double f(double x)
{
    return sqrt(sin(x)/(pow(cos(x), 9)));
}

void print_results(size_t count, std::vector<int>& M, std::vector<double>& vals, std::vector<int>& times)
{
    std::cout << std::setw(5) << "M" << std::setw(17) << "val" << std::setw(20) << "Time elapsed, ms\n";
    for(int i = 0; i < count; i++)
    {
        std::cout << std::setw(5) << M[i] << std::setw(17) <<  std::fixed << std::setprecision(13) << vals[i] 
            << std::setw(20) << std::setprecision(5) << times[i] << std::endl;
    }
}

int main()
{
    const double a = 0;
    const double b = 0.5;
    const double EPS = 3e-11;
    const int THREAD_COUNT = 30;

    std::vector<int> M = {2, 3, 4, 5, 6, 7, 8};
    int M_count = M.size();
    std::vector<int> calc_times(10);

    std::vector<double> I_values(10);
    for(int j = 0; j < M_count; j++)
    {
        std::cout << "M = " << M[j] << std::endl;
        auto start = std::chrono::steady_clock::now();
        I_values[j] = 0;
        double substep = (b-a)/M[j];
        for(int i = 0; i < M[j]; i++)
        {
            std::cout << "\t" << i + 1 << " interval\n";
            I_values[j] += simpson_parallel_runge(f, a + i*substep, a + (i+1)*substep, EPS/M[j], THREAD_COUNT);
        }
        auto end = std::chrono::steady_clock::now();
        calc_times[j] = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    }
    std::cout << "--------------------------------------\n";

    print_results(M_count, M, I_values, calc_times);

    return 0;
}
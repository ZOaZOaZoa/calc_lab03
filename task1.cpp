#include <iostream>
#include <iomanip>
#include <math.h>

#include "int_functions.h"

double int_right_parallel(double f(double), double a, double b, double h, int thread_count);
double int_right(double f(double), double a, double b, double h, double sum);
double diff_right(double f(double), double x0, double h);

void print_results(size_t count, std::vector<double>& h, std::vector<double>& vals, double precise_val)
{
    std::cout << std::setw(14) << "h" << std::setw(17) << "val" << std::setw(14) << "delta\n";
    for(int i = 0; i < count; i++)
    {
        std::cout << std::setw(14) << std::setprecision(3) << h[i] << std::setw(17) <<  std::fixed << std::setprecision(10) << vals[i]
                                            << std::setw(14) << std::scientific << std::setprecision(3) << abs(precise_val-vals[i]) << std::endl;
    }
}


int main()
{
    //Integration
    const double J = 77/(2*log(4)) - 15/(2*pow(log(4), 2));
    const double K = 15;
    const double I_K = 10;
    const double a = 0;
    const double b = 1;
    const int THREAD_COUNT = 1000;

    std::vector<double> h(K);

    for(int i = 1; i <=K; i++)
    {
        h[i-1] = (b-a)/pow(10, i);
    }

    std::vector<double> Ik(I_K);

    for(int i = 0; i < I_K; i++)
    {
        std::cout << "h=" << h[i] << std::endl;
        Ik[i] = int_right_parallel(f, a, b, h[i], THREAD_COUNT);
    }

    std::cout << "--------------------------------------\n";

    std::cout << "Integration. Precise value: " << std::setprecision(9) <<  J << std::endl;

    print_results(I_K, h, Ik, J);
    
    std::cout << std::fixed << "--------------------------------------\n";
    
    //Differentiation
    const double D_K = 15;
    const double X0 = 1;
    double D_precise = f_prime(X0);

    std::cout << "Differentiation. Precise value: " << std::setprecision(19) << D_precise << std::endl;

    std::vector<double> d_vals(D_K);

    for(int i = 0; i < D_K; i++)
    {
        d_vals[i] = diff_right(f, X0, h[i]);
    }
    
    print_results(D_K, h, d_vals, D_precise);

    //Оценить размер шага при котором начинает расти погрешность

    return 0;
}

double int_right_parallel(double f(double), double a, double b, double h, int thread_count)
{
    double sum = sum_interval_parallel(a, b, h, thread_count);
    
    return int_right(f, a, b, h, sum);
}

//Given sum from a to b. Subtract last value for right rect.
double int_right(double f(double), double a, double b, double h, double sum)
{
    return h*(sum - f(a));
}

double diff_right(double f(double), double x0, double h)
{
    return (f(x0+h)-f(x0))/h;
}
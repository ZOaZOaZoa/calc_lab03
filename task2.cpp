#include <iostream>
#include <math.h>
#include <vector>
#include <iomanip>
#include <chrono>

#include "int_functions.h"

double f(double x)
{
    return (2*x + 3)*pow(4, 2*x);
}

void print_results(size_t count, std::vector<double>& h, std::vector<double>& vals, double precise_val)
{
    std::cout << std::setw(14) << "h" << std::setw(17) << "val" << std::setw(14) << "delta\n";
    for(int i = 0; i < count; i++)
    {
        std::cout << std::setw(14) << std::setprecision(3) << h[i] << std::setw(17) <<  std::fixed << std::setprecision(10) << vals[i]
                                            << std::setw(14) << std::scientific << std::setprecision(3) << abs(precise_val-vals[i]) << std::endl;
    }
}

void print_results(size_t count, std::vector<double>& h, std::vector<double>& vals, std::vector<double>& error)
{
    std::cout << std::setw(14) << "h" << std::setw(17) << "val" << std::setw(14) << "runge\n";
    for(int i = 0; i < count; i++)
    {
        std::cout << std::setw(14) << std::setprecision(3) << h[i] << std::setw(17) <<  std::fixed << std::setprecision(10) << vals[i];

        if(i != 0)
        {
            std::cout << std::setw(14) << std::scientific << std::setprecision(3) << error[i] << std::endl;
        }             
        else
        {
            std::cout << std::endl;
        }                       
        
    }
}

void print_elapsed_time(std::chrono::steady_clock::time_point start, std::chrono::steady_clock::time_point end)
{
    std::cout << "Elapsed time: " << std::fixed 
            << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << "ms." << std::endl;
}


int main()
{
    double EPS = 3e-11;
    const double J = 77/(2*log(4)) - 15/(2*pow(log(4), 2));
    const double K = 15;
    const double I_K = 10;
    const double a = 0;
    const double b = 1;
    const int THREAD_COUNT = 96;

    std::vector<double> h(K);

    for(int i = 1; i <=K; i++)
    {
        h[i-1] = (b-a)/pow(10, i);
    }

    /*//Testing speed of algorithm on different values of THREAD_COUNT
    for(int thread_count = 50; thread_count < 1001; thread_count++)
    {
        auto start = std::chrono::steady_clock::now();

        std::vector<double> Ik(I_K);
        for(int i = 0; i < I_K; i++)
        {
            //std::cout << "h=" << h[i] << std::endl;
            Ik[i] = simpson_parallel(f, a, b, h[i], thread_count);
        }

        auto end = std::chrono::steady_clock::now();
        std::cout << "thread_count: " << std::setw(3) << thread_count << " Elapsed time: " << std::fixed 
            << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << "ms." << std::endl;
    }
    */

    auto start = std::chrono::steady_clock::now();

    std::vector<double> Ik(I_K);

    //f defined in int_functions.cpp
    for(int i = 0; i < I_K; i++)
    {
        std::cout << "h=" << h[i] << std::endl;
        Ik[i] = simpson_parallel(f, a, b, h[i], THREAD_COUNT);
    }
    auto end = std::chrono::steady_clock::now();

    std::cout << "--------------------------------------\n";

    std::cout << "Integration. Precise value: " << std::setprecision(15) <<  J << std::endl;
    print_results(I_K, h, Ik, J);
    print_elapsed_time(start, end);

    std::cout << "--------------------------------------\n";

    std::cout << "Integration with EPS precision. Precise value: " << std::setprecision(15) << std::fixed <<  J 
                << ". EPS: " << std::scientific << std::setprecision(2) << EPS <<  std::endl;

    const size_t MAX_ITER = 20;
    const size_t P = 4;

    start = std::chrono::steady_clock::now();

    h = std::vector<double>(MAX_ITER);
    h[0] = 0.1;
    std::vector<double> I(MAX_ITER);
    I[0] = simpson_parallel(f, a, b, h[0], THREAD_COUNT);
    std::vector<double> rung(MAX_ITER);
    rung[0] = 0;
    size_t it = 1;
    
    do
    {
        h[it] = h[it-1]/2;
        I[it] = simpson_parallel(f, a, b, h[it], THREAD_COUNT);
        rung[it] = (I[it]-I[it-1])/(pow(2, P) - 1);
    } while (it < MAX_ITER & fabs(rung[it++]) > EPS);

    end = std::chrono::steady_clock::now();
    
    print_results(it, h, I, rung);
    print_elapsed_time(start, end);

    return 0;
}
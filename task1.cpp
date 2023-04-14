#include <iostream>
#include <math.h>
#include <vector>
#include <iomanip>

double int_right(double f(double), double a, double b, double h);
double int_right(double f(double), double a, double b, double h, double sum);

double f(double x)
{
    return (2*x + 3)*pow(4, 2*x);
}

double sum_interval(double start, double end, double step)
{
    double sum = 0;
    for(int i = 0; i <= (end-start)/step; i++)
    {
        sum += f(start + i*step);
    }
    return sum;
}


int main()
{
    const double J = 23.8693127;
    const double K = 8;
    const double a = 0;
    const double b = 1;

    std::vector<double> h(K);

    for(int i = 1; i <=K; i++)
    {
        h[i-1] = (b-a)/pow(10, i);
    }

    std::vector<double> Ik(K);

    for(int i = 0; i < K; i++)
    {
        std::cout << "h=" << h[i] << std::endl;
        Ik[i] = int_right(f, a, b, h[i]);
    }

    std::cout << std::setw(14) << "h" << std::setw(17) << "I" << std::setw(14) << "delta\n";
    for(int i = 0; i < K; i++)
    {
        std::cout << std::setw(14) << std::fixed << h[i] << std::setw(17) << std::setprecision(10) << Ik[i]
                                            << std::setw(14) << std::scientific << std::setprecision(3) << abs(J-Ik[i]) << std::endl;
    }
    
    return 0;
}

double int_right(double f(double), double a, double b, double h)
{
    double sum = sum_interval(a, b, h);
    
    return int_right(f, a, b, h, sum);
}

double int_right(double f(double), double a, double b, double h, double sum)
{
    return h*(sum - f(a));
}


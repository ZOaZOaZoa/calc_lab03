#include <iostream>
#include <math.h>
#include <vector>
#include <iomanip>

double int_right(double f(double), double a, double b, double h);
double int_right(double f(double), double a, double b, double h, double sum);
double diff_right(double f(double), double x0, double h);

double f(double x)
{
    return (2*x + 3)*pow(4, 2*x);
}

double f_prime(double x)
{
    return 2*pow(4, 2*x)*(1+log(4)*(2*x+3));
}

double sum_interval(double start, double end, double step)
{
    double sum = 0;
    for(int i = 0; i <= std::round((end-start)/step); i++)
    {
        sum += f(start + i*step);
    }
    return sum;
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


int main()
{
    //Integration
    /*
    Integration. Precise value: 23.8693127
             h              val        delta
           0.1    27.9218971553     4.053e+00
     1.000e-02    24.2563417431     3.870e-01
     1.000e-03    23.9078329885     3.852e-02
     1.000e-04    23.8731629006     3.850e-03
     1.000e-05    23.8688976998     4.150e-04
     1.000e-06    23.8693511977     3.850e-05
     1.000e-07    23.8693165477     3.848e-06
     1.000e-08    23.8693130827     3.827e-07
     */
    const double J = 23.8693127;
    const double K = 15;
    const double I_K = 8;
    const double a = 0;
    const double b = 1;

    std::vector<double> h(K);

    for(int i = 1; i <=K; i++)
    {
        h[i-1] = (b-a)/pow(10, i);
    }

    std::vector<double> Ik(I_K);

    for(int i = 0; i < I_K; i++)
    {
        std::cout << "h=" << h[i] << std::endl;
        Ik[i] = int_right(f, a, b, h[i]);
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

double int_right(double f(double), double a, double b, double h)
{
    double sum = sum_interval(a, b, h);
    
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
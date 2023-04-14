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
    for(int i = 0; i <= (end-start)/step; i++)
    {
        sum += f(start + i*step);
    }
    return sum;
}


int main()
{
    //Integration
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

    std::cout << "Integration. Precise value: " << J << std::endl;

    std::cout << std::setw(14) << "h" << std::setw(17) << "I" << std::setw(14) << "delta\n";
    for(int i = 0; i < I_K; i++)
    {
        std::cout << std::setw(14) << h[i] << std::setw(17) <<  std::fixed << std::setprecision(10) << Ik[i]
                                            << std::setw(14) << std::scientific << std::setprecision(3) << abs(J-Ik[i]) << std::endl;
    }
    
    std::cout << std::fixed << "--------------------------------------\n";
    
    //Differentiation
    const double D_K = 15;
    const double X0 = 1;
    double D_precise = f_prime(X0);

    std::cout << "Differentiation. Precise value: " << D_precise << std::endl;

    std::vector<double> d_vals(D_K);

    for(int i = 0; i < D_K; i++)
    {
        d_vals[i] = diff_right(f, X0, h[i]);
    }
    
    std::cout << std::setw(14) << "h" << std::setw(17) << "D" << std::setw(14) << "delta\n";
    for(int i = 0; i < D_K; i++)
    {
        std::cout << std::setw(14) << h[i] << std::fixed << std::setw(17) << std::setprecision(10) << d_vals[i]
                                            << std::setw(14) << std::scientific << std::setprecision(3) << abs(D_precise - d_vals[i]) << std::endl;
    }


    //Оценить размер шага при котором начинает расти погрешность

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

double diff_right(double f(double), double x0, double h)
{
    return (f(x0+h)-f(x0))/h;
}
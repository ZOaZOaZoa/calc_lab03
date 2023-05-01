#include <iostream>
#include <vector>
#include <numeric>
#include <execution>
#include <math.h>

double func(double x) {
    return (2*x + 3)*pow(4, 2*x);
}

double sum_func_vals(double func(double), double a, double b, double h)
{
    int N = static_cast<int>(fabs(b-a)/h) + 1;
    double H = fabs(b-a)/(10);
    std::cout << N << std::endl;
    std::vector<double> vec(10);
    std::generate_n(vec.begin(), vec.size(), [n = a, H]() mutable { return H*n++; });
    for(auto val : vec)
    {
        std::cout << val << " ";
    }
    std::cout << "\nVector generated\n";

    double sum = std::transform_reduce(std::execution::par,
        vec.begin(), vec.end(), 0.0,
        [](double a, double b) { return a + b; },
        [func](double x) { return func(x); });
        
    return sum;
}


int main() {
    double h = 1e-9;
    double sum = sum_func_vals(func, 0, 1-h, h);

    std::cout << "Sum of function values: " << h*sum << std::endl;
}
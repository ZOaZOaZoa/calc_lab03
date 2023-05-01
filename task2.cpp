#include <iostream>
#include <math.h>
#include <vector>
#include <iomanip>
#include <chrono>
#include <omp.h>

double f(double x)
{
    return (2*x + 3)*pow(4, 2*x);
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

double simpson(double f(double), double a, double b, double h)
{
    double sum_x = sum_interval(a + h, b - h, h);

    double sum_halfX = sum_interval(a + h/2, b - h/2, h);

    return h/6*(f(a) + 4*sum_halfX + 2*sum_x + f(b));
}

double simpson_parallel(double f(double), double a, double b, double h, int thread_count)
{
    double sum_x = sum_interval_parallel(a + h, b - h, h, thread_count);

    double sum_halfX = sum_interval_parallel(a + h/2, b - h/2, h, thread_count);

    return h/6*(f(a) + 4*sum_halfX + 2*sum_x + f(b));
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

int main()
{
    double EPS = 3e-11;
    const double J = 77/(2*log(4)) - 15/(2*pow(log(4), 2));
    const double K = 15;
    const double I_K = 9;
    const double a = 0;
    const double b = 1;
    const int THREAD_COUNT = 728;

    std::vector<double> h(K);

    for(int i = 1; i <=K; i++)
    {
        h[i-1] = (b-a)/pow(10, i);
    }

    /*//Testing speed of algorithm on different values of thread_count
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

    for(int i = 0; i < I_K; i++)
    {
        std::cout << "h=" << h[i] << std::endl;
        Ik[i] = simpson_parallel(f, a, b, h[i], THREAD_COUNT);
    }

    auto end = std::chrono::steady_clock::now();

    std::cout << "--------------------------------------\n";

    std::cout << "Integration. Precise value: " << std::setprecision(15) <<  J << std::endl;

    print_results(I_K, h, Ik, J);
    std::cout << "Elapsed time: " << std::fixed 
            << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << "ms." << std::endl;

    std::cout << "--------------------------------------\n";

    std::cout << "Integration with EPS precision. Precise value: " << std::setprecision(15) << std::fixed <<  J 
                << ". EPS: " << std::scientific << std::setprecision(2) << EPS <<  std::endl;

    const size_t MAX_ITER = 20;
    const size_t P = 4;
    h = std::vector<double>(MAX_ITER);
    h[0] = 0.1;
    std::vector<double> I(MAX_ITER);
    I[0] = simpson(f, a, b, h[0]);
    std::vector<double> rung(MAX_ITER);
    rung[0] = 0;
    size_t it = 1;
    
    do
    {
        h[it] = h[it-1]/2;
        I[it] = simpson(f, a, b, h[it]);
        rung[it] = (I[it-1]-I[it])/(pow(2, P) - 1);
    } while (it < MAX_ITER & fabs(rung[it++]) > EPS);
    
    print_results(it, h, I, rung);

    return 0;
}
// TBB example showing the use of a std::mutex
// in combination with a TBB parallel for

// Simple TBB parallel_for example, allowing for some scaling timing tests

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <mutex>

#include "tbb/tbb.h"

#define SIZE 100000000

std::mutex mtx;

// Fill our array with a set of numbers (in this exercise
// there's no need for random values)
void set_x(double x[], size_t const n) {
    for (size_t i=0; i<n; ++i)
        x[i] = i+0.5;
}

// Take the log of every value
void serial_log(double x[], size_t n, double &sum) {
    for (size_t i=0; i<n; ++i) {
        x[i]=log(x[i]);
        sum += x[i];
    }
}

// Class for TBB to take the log in parallel
class parallel_log {
    double *const my_x;
    mutable double *my_sum; // Violate const for this exercise as we're
                            // really doing a parallel reduce (non-const)
    public:
        void operator() (tbb::blocked_range<size_t>& r) const {
            double *x = my_x;
            double _sum{0.0};
            for(size_t i=r.begin(); i!=r.end(); ++i) {
                x[i] = log(x[i]);
                _sum += x[i];
            }
            std::lock_guard<std::mutex> my_lock (mtx);
            *my_sum += _sum;
        }

        parallel_log(double x[], double &sum):
            my_x{x}, my_sum{&sum}
        {}
};


int main(int argc, char *argv[]) {
    size_t my_size = SIZE;
    if (argc == 2) {
        my_size = atoi(argv[1]);
    }

    std::cout << "Array size is " << my_size << std::endl;

    double *x = new double[my_size];
    set_x(x, my_size);

    // Do the serial loop
    double sum{0.0};
    tbb::tick_count t0 = tbb::tick_count::now();
    serial_log(x, my_size, sum);
    tbb::tick_count t1 = tbb::tick_count::now();
    auto serial_tick_interval = t1-t0;
    std::cout
        << "Serial loop took "
        << serial_tick_interval.seconds()
        << "s (sum " << sum << ")"
        << std::endl;

    // Do the parallel loop
    set_x(x, my_size);
    t0 = tbb::tick_count::now();
    sum = 0.0;
    tbb::parallel_for(tbb::blocked_range<size_t>(0, my_size),
        parallel_log(x, sum));
    t1 = tbb::tick_count::now();
    auto parallel_tick_interval = t1-t0;
    std::cout
        << "Parallel loop took "
        << parallel_tick_interval.seconds()
        << "s (sum " << sum << ")"
        << std::endl;

    std::cout
        << "Parallel speedup: "
        << serial_tick_interval.seconds()/parallel_tick_interval.seconds()
        << "x"
        << std::endl;

    delete[] x;

    return 0;
}

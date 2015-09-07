#include <iostream>
#include <cmath>
#include <random>
#include <chrono>

#include "tbb/tbb.h"

#define SIZE 100000000

// Fill our array with a set of numbers (in this exercise
// there's no need for random values)
void set_x(double x[], size_t const n) {
    for (size_t i=0; i<n; ++i)
        x[i] = i+0.5;
}

// Take the log of every value
void serial_log(double x[], size_t n) {
    for (size_t i=0; i<n; ++i)
        x[i]=log(x[i]);
}

// Class for TBB to take the log in parallel
class parallel_log {
    double *const my_x;
    public:
        void operator() (tbb::blocked_range<size_t>& r) const {
            double *x = my_x;
            for(size_t i=r.begin(); i!=r.end(); ++i) {
                x[i] = log(x[i]);
            }
        }

        parallel_log(double x[]):
            my_x{x}
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
    tbb::tick_count t0 = tbb::tick_count::now();
    serial_log(x, my_size);
    tbb::tick_count t1 = tbb::tick_count::now();
    auto serial_tick_interval = t1-t0;
    std::cout
        << "Serial loop took "
        << serial_tick_interval.seconds()
        << "s"
        << std::endl;

    // Do the parallel loop
    set_x(x, my_size);
    t0 = tbb::tick_count::now();
    tbb::parallel_for(tbb::blocked_range<size_t>(0, my_size), parallel_log(x));
    t1 = tbb::tick_count::now();
    auto parallel_tick_interval = t1-t0;
    std::cout
        << "Parallel loop took "
        << parallel_tick_interval.seconds()
        << "s"
        << std::endl;

    std::cout
        << "Parallel speedup: "
        << serial_tick_interval.seconds()/parallel_tick_interval.seconds()
        << "x"
        << std::endl;

    return 0;
}

// TBB Parallel Minimisation, using parallel reduce

#include <iostream>
#include <limits>
#include <cmath>
#include <chrono>

#include "tbb/tbb.h"

// The function to minimise
double f(double x) {
    return std::exp(x) - std::pow(x, 3);
}

class MinIndexF {
    const double *const my_x;
public:
    double value_of_min;
    long index_of_min;
    void operator()( const tbb::blocked_range<size_t>& r ) {
        const double *x = my_x;
        for( size_t i=r.begin(); i!=r.end(); ++i ) {
           double value = f(x[i]);    
           if( value<value_of_min ) {
               value_of_min = value;
               index_of_min = i;
           }
        }
    }
 
    MinIndexF( MinIndexF& m, tbb::split ) : 
        my_x(m.my_x), 
        value_of_min(std::numeric_limits<double>::max()),
        index_of_min(-1) 
   {}
 
    void join( const MinIndexF& n ) {
        if( n.value_of_min<value_of_min ) {
            value_of_min = n.value_of_min;
            index_of_min = n.index_of_min;
        }
    }
             
    MinIndexF( const double x[] ) :
        my_x(x), 
        value_of_min(std::numeric_limits<double>::max()),
        index_of_min(-1)
    {}
};


int main() {
    const double x_min = -10.0;
    const double x_max =  10.0;
    const size_t n = 100000;

    double *x = new double[n];
    for (size_t i = 0; i < n; ++i) x[i] = x_min + (x_max - x_min) / n * i;

    auto min_finder = MinIndexF(x);
    tbb::parallel_reduce(tbb::blocked_range<size_t>(0,n), min_finder);

    std::cout << "Function f minimised at x=" << x[min_finder.index_of_min] 
    << "; f(x_min)=" << min_finder.value_of_min << std::endl;

    delete[] x;

    return 0;
}
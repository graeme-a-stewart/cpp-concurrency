// Simple TBB parallel reduction example

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>

#include "tbb/tbb.h"
#include "tutorialutils.h"

#define SIZE 100000

// Work function - define the work to do here
// so that it can be dropped into both the
// serial and parallel codes
inline double do_work(size_t i) {
  return burn(1000);
}

// Sum up values in a serial way
double serial_sum(double x[], size_t n) {
  double answer=0.0;
  for (size_t i=0; i<n; ++i) {
    x[i] = do_work(i);
    answer += x[i];
  }
  return answer;
}

// Class for TBB parallel summation
class parallel_sum {
  double *const my_x;
public:
  double answer;
  void operator() (tbb::blocked_range<size_t>& r) {
    double *x = my_x;
    for(size_t i=r.begin(); i!=r.end(); ++i) {
      x[i] = do_work(i);
      answer += x[i];
    }
  }

  void join(parallel_sum& b) {
    answer+=b.answer;
  }

  parallel_sum(parallel_sum& a, tbb::split):
    my_x{a.my_x}, answer{0.0} {}

    parallel_sum(double x[]):
      my_x{x} {}
};


int main(int argc, char *argv[]) {
  size_t my_size = SIZE;
  if (argc == 2) {
    my_size = atoi(argv[1]);
  }

  std::cout << "Array size is " << my_size << std::endl;

  double *x = new double[my_size];

  // Do the serial loop.
  tbb::tick_count t0 = tbb::tick_count::now();
  double s_sum = serial_sum(x, my_size);
  tbb::tick_count t1 = tbb::tick_count::now();
  auto serial_tick_interval = t1-t0;
  std::cout << "Serial loop took "
      << serial_tick_interval.seconds()
      << "s"
      << std::endl;

  // Do the parallel loop.
  t0 = tbb::tick_count::now();
  parallel_sum ps(x);
  tbb::parallel_reduce(tbb::blocked_range<size_t>(0, my_size), ps);
  double p_sum = ps.answer;
  t1 = tbb::tick_count::now();
  auto parallel_tick_interval = t1-t0;
  std::cout << "Parallel loop took "
      << parallel_tick_interval.seconds()
      << "s"
      << std::endl;
  std::cout << "Serial answer: "
      << s_sum
      << "; parallel answer: "
      << p_sum
      << std::endl;
  std::cout << "Parallel speedup: "
      << serial_tick_interval.seconds()/parallel_tick_interval.seconds()
      << "x" << std::endl;

  delete[] x;

  return 0;
}

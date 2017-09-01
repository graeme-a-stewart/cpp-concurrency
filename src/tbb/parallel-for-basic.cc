// Simple TBB parallel_for example, allowing for some scaling timing tests

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
  return burn(100);
}

// Take the log of every value
void serial_work(double x[], size_t n) {
  for (size_t i=0; i<n; ++i)
    x[i]=do_work(i);
}

// Class for TBB to take the log in parallel
class parallel_work {
  double *const my_x;
public:
  void operator() (tbb::blocked_range<size_t>& r) const {
    double *x = my_x;
    // std::cout << "Working on block of " << r.end() - r.begin() << std::endl;
    for(size_t i=r.begin(); i!=r.end(); ++i) {
      x[i] = do_work(i);
    }
  }

  parallel_work(double x[]):
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

  // Do the serial loop
  tbb::tick_count t0 = tbb::tick_count::now();
  serial_work(x, my_size);
  tbb::tick_count t1 = tbb::tick_count::now();
  auto serial_tick_interval = t1-t0;
  std::cout << "Serial loop took "
      << serial_tick_interval.seconds()
      << "s"
      << std::endl;

  // Do the parallel loop
  t0 = tbb::tick_count::now();
  tbb::parallel_for(tbb::blocked_range<size_t>(0, my_size),
      parallel_work(x));
  t1 = tbb::tick_count::now();
  auto parallel_tick_interval = t1-t0;
  std::cout << "Parallel loop took "
      << parallel_tick_interval.seconds()
      << "s"
      << std::endl;

  std::cout << "Parallel speedup: "
      << serial_tick_interval.seconds()/parallel_tick_interval.seconds()
      << "x"
      << std::endl;

  delete[] x;

  return 0;
}

// Simple TBB parallel for using a lambda

#include <iostream>
#include <cmath>
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

// Take the log of every value
void serial_work(double x[], size_t n) {
  for (size_t i=0; i<n; ++i)
    x[i]=do_work(i);
}

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
  // Here we just pass a lambda function that captures local variables by value: [=]
  tbb::parallel_for(
      tbb::blocked_range<size_t>(0, my_size),
      [=](tbb::blocked_range<size_t>& r) {
    for(size_t i=r.begin(); i!=r.end(); ++i)
      x[i] = do_work(i);
  });
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

  // Slightly modified parallel loop, using the compact notation
  t0 = tbb::tick_count::now();
  // Here we just pass a lambda function that captures local variables by value: [=]
  tbb::parallel_for(size_t(0), my_size, [=](size_t i) {x[i] = do_work(i);});
  t1 = tbb::tick_count::now();
  parallel_tick_interval = t1-t0;
  std::cout << "Compact parallel loop took "
      << parallel_tick_interval.seconds()
      << "s"
      << std::endl;
  std::cout << "Compact parallel speedup: "
      << serial_tick_interval.seconds()/parallel_tick_interval.seconds()
      << "x"
      << std::endl;
  delete[] x;

  return 0;
}



// Collection of a few useful functions for the TBB
// concurrency tutorial

#include <cmath>
#include <chrono>

double burn(unsigned long iterations = 10'000'000lu) {
  // Perform a time wasting bit of maths
  double volatile sum{0.0};
  double f;
  for (auto i = 0lu; i < iterations; ++i) {
    f = (double)(i+1) / iterations * 1.414;
    sum += std::sin(std::log(f));
  }
  return sum;
}

double burn_for(float ms_interval) {
  // The compiler is not usually able to optimise away this
  // null-op calculation, but assigning the result to a
  // volatile variable guarantees that.
  volatile double burn_result{0.0};
  std::chrono::duration<float, std::milli> chrono_interval(ms_interval);
  auto start = std::chrono::system_clock::now();
  while (std::chrono::system_clock::now() - start < chrono_interval)
    burn_result += burn(20);

  return burn_result;
}


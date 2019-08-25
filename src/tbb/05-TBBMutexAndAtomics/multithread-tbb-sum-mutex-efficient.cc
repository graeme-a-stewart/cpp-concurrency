// Program showing how multithreaded summing can be fixed with
// mutexes.
// N.B. This example is artificial as the solution here would
// actually be a tbb:parallel_reduce.

#include <thread>
#include <iostream>
#include <random>
#include <vector>
#include <mutex>

#include "tbb/tbb.h"

int total_entries;
float sum;
size_t occupancy;

#define SIZE 100000000

void fill_detector(std::vector<float> &det, size_t n=SIZE) {
  std::default_random_engine generator;
  std::uniform_real_distribution<float> distribution(-0.2,1.0);

  if (!det.empty())
    det.erase(det.begin(), det.end());
  det.reserve(n);

  for (size_t i=0; i<n; ++i)
    det.push_back(distribution(generator));
}

size_t serial_occupancy(std::vector<float> &det) {
  size_t sum=0;
  for (auto& el: det) {
    if (el > 0.0f) {
      ++sum;
    }
  }
  return sum;
}


int main(int argc, char* argv[]) {
  int chunk = 10000; // default chunk size
  if (argc==2) {
    chunk = std::stoi(argv[1]);
  }

  std::vector<float> det;

  fill_detector(det, SIZE);

  tbb::tick_count t0, t1;
  tbb::tick_count::interval_t serial_tick_interval, parallel_tick_interval;

  // Serial calculation
  t0 = tbb::tick_count::now();
  std::cout << "Occupancy is " << serial_occupancy(det) << std::endl;
  t1 = tbb::tick_count::now();
  serial_tick_interval = t1 - t0;
  std::cout << "Serial occupancy took "
    << serial_tick_interval.seconds() << "s" << std::endl;

  // Multithreaded calculation
  std::mutex mtx;
  occupancy = 0;
  t0 = tbb::tick_count::now();
  tbb::parallel_for(0, SIZE, chunk, [=, &det, &mtx](int i){
    // std::cout << "Local i = " << i << std::endl;
    size_t local_sum = 0;
    for (size_t j=0; j<chunk; ++j)
      if (det[i+j] > 0.0f) ++local_sum;
    std::lock_guard<std::mutex> lck(mtx); 
    occupancy += local_sum;
    });
  t1 = tbb::tick_count::now();
  parallel_tick_interval = t1 - t0;
  std::cout << "Multi-thread occupancy is " << occupancy 
    << " with a chunk size of " << chunk << "."
    << std::endl;
  std::cout << "Multi-thread occupancy took "
    << parallel_tick_interval.seconds() << "s" << std::endl;
  std::cout << "Speed up: " 
    << serial_tick_interval.seconds()/parallel_tick_interval.seconds() << "x" << std::endl;

  return 0;
}

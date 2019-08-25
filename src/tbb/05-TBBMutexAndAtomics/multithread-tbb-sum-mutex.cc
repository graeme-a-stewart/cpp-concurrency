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


int main() {
  std::vector<float> det;

  fill_detector(det, SIZE);

  // Serial calculation
  std::cout << "Occupancy is " << serial_occupancy(det) << std::endl;

  // Multithreaded calculation
  std::mutex mtx;
  occupancy = 0;
  tbb::parallel_for(0, SIZE, 1, [=, &det, &mtx](int i){
    std::lock_guard<std::mutex> lck(mtx); 
    if (det[i] > 0.0f) ++occupancy;
    });
  std::cout << "Multi-thread occupancy is " << occupancy << std::endl;

  return 0;
}

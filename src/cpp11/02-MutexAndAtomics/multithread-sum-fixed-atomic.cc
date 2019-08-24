#include <thread>
#include <atomic>
#include <iostream>
#include <random>
#include <vector>
#include <chrono>

int total_entries;
float sum;
std::atomic<size_t> occupancy;

#define SIZE 100000000
#define THREAD_POOL 50

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

// This function looks at the occupancy of an element in the detector
// using the global variable occupancy to allow the sum to happen 
// across threads
void partial_occupancy(std::vector<float> &det, size_t begin, size_t end) {
  for (size_t i=begin; i<end; ++i) {
    if (det[i] > 0.0f) {
      ++occupancy;
    }
  }
}


int main() {
  std::vector<float> det;

  fill_detector(det, SIZE);

  // Serial calculation
  std::cout << "Occupancy is " << serial_occupancy(det) << std::endl;

  // Multithreaded calculation
  auto start = std::chrono::high_resolution_clock::now();
  occupancy = 0;
  std::thread pool[THREAD_POOL];
  size_t chunk = SIZE/THREAD_POOL;

  for (size_t t=0; t<THREAD_POOL; ++t) {
    pool[t] = std::thread(partial_occupancy, std::ref(det), chunk*t, chunk*(t+1));
  }

  for (int t=0; t<THREAD_POOL; ++t)
    pool[t].join();
  auto end = std::chrono::high_resolution_clock::now();
  auto duration = end - start;

  std::cout << "Multi-thread occupancy is " << occupancy << " (took " << std::chrono::duration<float, std::milli> (duration).count() << "ms)" << std::endl;

  return 0;
}

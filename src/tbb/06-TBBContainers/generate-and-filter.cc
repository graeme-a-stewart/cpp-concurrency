#include <iostream>
#include <random>
#include <chrono>
#include <thread>

#include "tbb/tbb.h"

static size_t default_size = 100000000;
static size_t default_grain_size = 0;

class filler {
private:
  tbb::concurrent_vector<float> *const my_vector;
  
public:
  void operator() (tbb::blocked_range<size_t>& r) const {
    // This monster hashes the thread id with a high resolution epoch time to
    // ensure that each fill segment gets a ~unique seed (could also add a global
    // atomic counter if needed!)
    unsigned long seed = std::hash<std::thread::id>()(std::this_thread::get_id()) ^
      std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    std::default_random_engine generator(seed);
    std::uniform_real_distribution<float> distribution(-100.0, 1.0);
    
    auto insert_range_iterator = (*my_vector).grow_by(r.size());
    for (size_t count=0; count<r.size(); ++count) {
      *insert_range_iterator++ = distribution(generator);
    }
  }

  filler(tbb::concurrent_vector<float> *input_data_vector):
    my_vector{input_data_vector}
  {}
  
};

class filter {
private:
  tbb::concurrent_vector<float> *const my_input_vector, *const my_filtered_vector;
  
public:
  void operator() (tbb::blocked_range<size_t>& r) const {
    for(size_t i=r.begin(); i!=r.end(); ++i) {
      if ((*my_input_vector)[i] > 0.0) {
	(*my_filtered_vector).push_back((*my_input_vector)[i]);
      }
    }
  }

  filter(tbb::concurrent_vector<float> *input_data_vector, tbb::concurrent_vector<float> *output_filtered_vector):
    my_input_vector{input_data_vector}, my_filtered_vector{output_filtered_vector}
  {}
};


void print_vec(tbb::concurrent_vector<float> vec, bool detailed) {
  if (detailed) {
    for (auto el: vec) {
      std::cout << el << std::endl;
    }
  }
  std::cout << "There were " << vec.size() << " elements" << std::endl;
}

int main(int argc, char *argv[]) {
  size_t my_size = default_size;
  size_t my_grain_size = default_grain_size;
  if (argc >= 2) {
    my_size = atoi(argv[1]);
  }
  if (argc >= 3) {
    my_grain_size = atoi(argv[2]);
  }

  tbb::concurrent_vector<float> data_vector, filtered_vector;
  
  std::cout << "Target data array size is " << my_size << "; Grain size is " << my_grain_size << std::endl;

  tbb::tick_count t0 = tbb::tick_count::now();
  tbb::parallel_for(tbb::blocked_range<size_t>(0, my_size, my_grain_size), filler(&data_vector));
  tbb::tick_count t1 = tbb::tick_count::now();
  auto tick_interval = t1-t0;

  std::cout << "Filler took " << tick_interval.seconds() << "s" << std::endl;
  print_vec(data_vector, false);

  t0 = tbb::tick_count::now();
  tbb::parallel_for(tbb::blocked_range<size_t>(0, my_size, my_grain_size), filter(&data_vector, &filtered_vector));
  t1 = tbb::tick_count::now();
  tick_interval = t1-t0;
  
  std::cout << "Filter took " << tick_interval.seconds() << "s" << std::endl;  
  print_vec(filtered_vector, false);

  return 0;
}

#include <thread>
#include <atomic>
#include <iostream>
#include <random>
#include <vector>

int total_entries;
float sum;
size_t occupancy;

#define SIZE 10000
#define THREAD_POOL 10

class Calorimeter {
  // Class holding the data and manipulation methods for
  // a 1 dimensional calorimiter.
private:
  std::vector<double> m_cells;
  size_t m_detector_size;
  std::atomic<unsigned long> m_occupancy;

  void partial_occupancy(size_t const begin, size_t const end) {
    size_t local_occupancy=0;
    for (size_t i=begin; i<end; ++i)
      if (is_occupied(m_cells[i]))
	++local_occupancy;
    // Sync to the atomic as little as possible (it's expensive)
    m_occupancy += local_occupancy;
  }

  inline bool is_occupied(double const value) {
    if (value > 0.0)
      return true;
    return false;
  }
  

public:
  void random_fill() {
    // Fill the calo cells with random data (including noise)
    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(-0.2,1.0);

    if (!m_cells.empty())
      m_cells.erase(m_cells.begin(), m_cells.end());
    m_cells.reserve(m_detector_size);

    for (size_t i=0; i<m_detector_size; ++i)
      m_cells.push_back(distribution(generator));
  }

  size_t serial_occupancy() {
    // Calculate detector occupancy: the number of cells over the threshold of 0.0
    m_occupancy = 0;
    partial_occupancy(0, m_cells.size());
    return m_occupancy;
  }

  size_t parallel_occupancy(unsigned int const threads) {
    m_occupancy = 0;
    unsigned int const chunk = m_cells.size() / threads;  // This needs to divide with no remainder!

    std::vector<std::thread> pool;

    for (size_t t=0; t<threads; ++t) {
      // Note how to call a class method when spawing an std::thread
      pool.push_back(std::thread(&Calorimeter::partial_occupancy, this, t*chunk, (t+1)*chunk));
    }

    for (auto& thrd: pool)
      thrd.join();
    
    return m_occupancy;
  }

  Calorimeter(size_t const size):
    m_detector_size{size}, m_occupancy{0} {}
  
};
   

int main() {
  Calorimeter calo(SIZE);
  calo.random_fill();

  // Serial calculation
  std::cout << "Serial occupancy is " << calo.serial_occupancy() << std::endl;

  // Multithreaded calculation
  std::cout << "Multi-thread occupancy is " << calo.parallel_occupancy(THREAD_POOL) << std::endl;

  return 0;
}

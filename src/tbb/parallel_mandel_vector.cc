#include <iostream>
#include <vector>
#include <complex>
#include <tbb/tbb.h>

const unsigned int max_iter=256;
const size_t default_resolution=150;

bool inside_mandel(std::complex<float> c) {
  unsigned int iter=0;
  std::complex<float> z(0.0, 0.0);
  while (iter++ < max_iter) {
    z = z*z + c;
    if (std::norm(z) > 4.0f)
      return false;
  }
  return true;
}

class parallel_mandel {
private:
  std::vector<std::vector<bool>>* my_set;
  size_t resolution;

public:
  void operator()(tbb::blocked_range2d<size_t>& r) const {
    std::vector<std::vector<bool>>* set = my_set;
    for (size_t i=r.rows().begin(); i!=r.rows().end(); ++i) {
      for (size_t j=r.cols().begin(); j!=r.cols().end(); ++j) {
        std::complex<float> z0(double(i)/resolution * 4.0 - 2.0, double(j)/resolution * 4.0 - 2.0);
#ifdef DEBUG
        std::cout << "Trying " << z0.real() << "," << z0.imag() << " ";
#endif
        (*set)[i][j] = inside_mandel(z0);
#ifdef DEBUG
        std::cout << (*set)[i][j] << std::endl;
#endif
      }
    }
  }

  parallel_mandel(std::vector<std::vector<bool>>* set):
    my_set{set}, resolution{set->size()} {};
};

void serial_mandel(std::vector<std::vector<bool>>* set) {
  size_t resolution=set->size();
  for (size_t i=0; i<set->size(); ++i) {
    for (size_t j=0; j<(*set)[i].size(); ++j) {
      std::complex<float> z0(double(i)/resolution * 4.0 - 2.0, double(j)/resolution * 4.0 - 2.0);
      (*set)[i][j] = inside_mandel(z0);
    }
  }
}


int main(int argc, char* argv[]) {
  size_t res = default_resolution;
  if (argc==2) {
    res = std::stoul(argv[1]);
  }

  std::vector<std::vector<bool>> set;
  set.reserve(res);
  for (size_t i=0; i<res; ++i) {
    std::vector<bool> a(res, false);
    set.push_back(a);
  }

  tbb::tick_count t0 = tbb::tick_count::now();
  serial_mandel(&set);
  tbb::tick_count t1 = tbb::tick_count::now();
  auto serial_tick_interval = t1-t0;
  std::cout << "Serial mandel took " << serial_tick_interval.seconds() << "s" << std::endl;

  t0 = tbb::tick_count::now();
  tbb::parallel_for(tbb::blocked_range2d<size_t>(0, res, 0, res), parallel_mandel(&set));
  t1 = tbb::tick_count::now();
  auto parallel_tick_interval = t1-t0;
  std::cout << "Parallel mandel took " << parallel_tick_interval.seconds() << "s" << std::endl;

  std::cout << "TBB Parallel speed-up: " << serial_tick_interval.seconds()/parallel_tick_interval.seconds()
      << "s" << std::endl;

  // Now print the map of points
  if (res <= 150) {
    for (size_t i=0; i<res; ++i) {
      for (size_t j=0; j<res; ++j) {
        if (set[j][i])
          std::cout << " ";
        else
          std::cout << ".";
      }
      std::cout << std::endl;
    }
  }


  return 0;
}


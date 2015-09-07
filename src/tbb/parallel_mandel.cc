#include <iostream>
#include <vector>
#include <complex>
#include <tbb/tbb.h>

const unsigned int max_iter=256;
const size_t res=2000;
const float box=2.0;

bool in_mandel(std::complex<float> c) {
  std::complex<float> z(0.0, 0.0);
  unsigned int iter=0;
  bool in_set=true;
  while (in_set && (++iter < max_iter)) {
    z = z*z + c;
    if (std::abs(z) > 2.0)
      in_set=false;
  }
  return in_set;
}

void serial_mandel(bool (*set)[res]) {
    for (size_t i=0; i!=res; ++i) {
        for (size_t j=0; j!=res; ++j) {
            std::complex<float> c(
                double(i)/res * 2 * box - box,
                double(j)/res * 2 * box - box
            );
            set[i][j] = in_mandel(c);
        }
    }
}

class parallel_mandel {
    private:
        bool (*my_set)[res];
  
    public:
        void operator()(tbb::blocked_range2d<size_t>& r) const {
            bool (*set)[res] = my_set;
            for (size_t i=r.rows().begin(); i!=r.rows().end(); ++i) {
                for (size_t j=r.cols().begin(); j!=r.cols().end(); ++j) {
                    std::complex<float> c(
                        double(i)/res * 2 * box - box,
                        double(j)/res * 2 * box - box
                    );
                    set[i][j] = in_mandel(c);
                }
            }
        }

        parallel_mandel(bool set[res][res]):
            my_set{set} {};
};

int main() {
    bool set[res][res];

    tbb::tick_count t0 = tbb::tick_count::now();
    serial_mandel(set);
    tbb::tick_count t1 = tbb::tick_count::now();
    auto serial_tick_interval = t1-t0;
    std::cout
        << "Serial mandel took "
        << serial_tick_interval.seconds()
        << "s"
        << std::endl;

    t0 = tbb::tick_count::now();
    tbb::parallel_for(
        tbb::blocked_range2d<size_t>(0, res, 0, res),
        parallel_mandel(set)
    );
    t1 = tbb::tick_count::now();
    auto parallel_tick_interval = t1-t0;
    std::cout
        << "Parallel mandel took "
        << parallel_tick_interval.seconds()
        << "s"
        << std::endl;
  
    std::cout
        << "Parallel speedup: "
        << serial_tick_interval.seconds()/parallel_tick_interval.seconds()
        << "x" << std::endl;

    // Now print the map of points (if it's a reasonable size)
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

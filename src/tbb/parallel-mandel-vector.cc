// Mandelbrot set generator using 2D TBB blocked range
// Internally use a vector of bools for the result map, which allows
// for easier setting of a dynamic resolution
//
// parallel_mandel_vector [GRID SIZE] [RUN_SERIAL]

#include <iostream>
#include <vector>
#include <complex>
#include <tbb/tbb.h>

const unsigned int max_iter=256;
const size_t default_resolution=150;

// Function that returns true/false if a starting point is considered to be
// inside the Mandelbrot set (at least still inside after max_iter iterations)
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

// Class definition for TBB parallelised Mandelbrot set calculation
class parallel_mandel {
    private:
        std::vector<std::vector<bool>>* my_set;
        size_t resolution;

    public:
        void operator()(tbb::blocked_range2d<size_t>& r) const {
            std::vector<std::vector<bool>>* set = my_set;
            for (size_t i=r.rows().begin(); i!=r.rows().end(); ++i) {
                for (size_t j=r.cols().begin(); j!=r.cols().end(); ++j) {
                    std::complex<float> z0(
                        double(i)/resolution * 4.0 - 2.0,
                        double(j)/resolution * 4.0 - 2.0
                    );
                    #ifdef DEBUG
                        std::cout
                            << "Trying "
                            << z0.real()
                            << ","
                            << z0.imag()
                            << " ";
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

// Simple serial implementation of Mandelbrot set finder
void serial_mandel(std::vector<std::vector<bool>>* set) {
    size_t resolution=set->size();
    for (size_t i=0; i<set->size(); ++i) {
        for (size_t j=0; j<(*set)[i].size(); ++j) {
            std::complex<float> z0(
                double(i)/resolution * 4.0 - 2.0,
                double(j)/resolution * 4.0 - 2.0
            );
            (*set)[i][j] = inside_mandel(z0);
        }
    }
}

int main(int argc, char* argv[]) {
	bool do_serial = true;
    size_t res = default_resolution;
    if (argc>=2) {
        res = std::stoul(argv[1]);
    }
    if (argc==3 and argv[2][0] == 'n') {
    	do_serial = false;
    }
    std::cout << "Starting mandel on a grid of " << res << std::endl;

    std::vector<std::vector<bool>> set;
    set.reserve(res);
    for (size_t i=0; i<res; ++i) {
        std::vector<bool> a(res, false);
        set.push_back(a);
    }

    tbb::tick_count t0, t1;
    tbb::tick_count::interval_t serial_tick_interval, parallel_tick_interval;
    if (do_serial) {
		t0 = tbb::tick_count::now();
		serial_mandel(&set);
		t1 = tbb::tick_count::now();
		serial_tick_interval = t1-t0;
		std::cout
			<< "Serial mandel took "
			<< serial_tick_interval.seconds()
			<< "s"
			<< std::endl;
    } else {
    	std::cout << "Skipping serial calculation" << std::endl;
    }

    t0 = tbb::tick_count::now();
    tbb::parallel_for(
        tbb::blocked_range2d<size_t>(0, res, 0, res),
        parallel_mandel(&set)
    );
    t1 = tbb::tick_count::now();
    parallel_tick_interval = t1-t0;
    std::cout
        << "Parallel mandel took "
        << parallel_tick_interval.seconds()
        << "s" << std::endl;

    if (do_serial) {
		std::cout
			<< "TBB Parallel speed-up: "
			<< serial_tick_interval.seconds()/parallel_tick_interval.seconds()
			<< "s" << std::endl;
    }

  // Now print the map of points (if it's reasonable!)
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


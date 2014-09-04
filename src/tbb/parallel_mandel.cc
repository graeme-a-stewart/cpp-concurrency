#include <iostream>
#include <vector>
#include <complex>
#include <tbb/tbb.h>

const unsigned int max_iter=256;
const size_t res=150;

class parallel_mandel {
private:
  bool (*my_set)[res];
  
public:
  void operator()(tbb::blocked_range2d<size_t>& r) const {
    bool (*set)[res] = my_set;
    for (size_t i=r.rows().begin(); i!=r.rows().end(); ++i) {
      for (size_t j=r.cols().begin(); j!=r.cols().end(); ++j) {
	std::complex<float> z0(double(i)/res * 4.0 - 2.0, double(j)/res * 4.0 - 2.0);
#ifdef DEBUG
	std::cout << "Trying " << z0.real() << "," << z0.imag() << " ";
#endif
	std::complex<float> z(0.0, 0.0);
	unsigned int iter=0;
	bool inside=true;
	while (inside && (++iter < max_iter)) {
	  z = z*z + z0;
	  if (std::abs(z) > 2.0)
	    inside=false;
	}
#ifdef DEBUG
	std::cout << inside << std::endl;
#endif
	set[i][j] = inside;
      }
    }
  }
	  
  parallel_mandel(bool set[res][res]):
    my_set{set} {};
};


int main() {
  bool set[res][res];

  tbb::parallel_for(tbb::blocked_range2d<size_t>(0, res, 0, res), parallel_mandel(set));

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


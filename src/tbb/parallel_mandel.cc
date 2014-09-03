#include <iostream>
#include <vector>
#include <tbb/tbb.h>

const unsigned int max_iter=256;
const size_t res=100;

class parallel_mandel {
private:
  bool (*my_set)[res];
  
public:
  void operator()(tbb::blocked_range2d<size_t>& r) const {
    bool (*set)[res] = my_set;
    for (size_t i=r.rows().begin(); i!=r.rows().end(); ++i) {
      for (size_t j=r.cols().begin(); j!=r.cols().begin(); ++j) {
	double x0 = double(i)/res * 2.0 - 1.0;	
	double y0 = double(j)/res * 2.0 - 1.0;
	double x=x0;
	double y=y0;
	double last_x=x;
	double last_y=y;
	unsigned int iter=0;
	bool inside=true;
	while (inside && (++iter < max_iter)) {
	  x = last_x*last_x - last_y*last_y + x0;
	  y = 2*last_x*last_y + y0;
	  last_x = x;
	  last_y = y;
	  if (x*x + y*y > 4.0)
	    inside=false;
	}
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

  return 0;
}


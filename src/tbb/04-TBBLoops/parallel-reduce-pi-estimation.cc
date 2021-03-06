// illustration of pi estimation using TBB parallel_reduce
// Uses the Gregory & Leibniz series for PI:
// 4 * (1 - 1/3 + 1/5 - 1/7 + 1/9 - ...)

#include <iostream>
#include <iomanip>
#include <tbb/tbb.h>
#include <tbb/parallel_for.h>

class MySeries{
private:
  double my_sum;

public:
  MySeries(): my_sum(0){
  }

  MySeries(const MySeries &x, tbb::split): my_sum(0){
  }

  void operator()(const tbb::blocked_range<size_t>& r){
    double sum = my_sum;
    size_t end = r.end();
    for (long i = r.begin(); i != end; ++i){
      double denominator = i * 2.0 + 1.0;
      if (i % 2 == 1){
        denominator = -denominator;
      }
      sum += 1.0 / denominator;
    }
    my_sum = sum;
  }

  void join (MySeries& y){
    my_sum += y.my_sum;
  }

  const double get_sum() {return my_sum;}
};

int main() {
  MySeries x;
  tbb::parallel_reduce(tbb::blocked_range<size_t>(0, 1000000000), x);
  std::cout << std::setprecision(14) << 4 * x.get_sum() << std::endl;
  return 0;
}


// illustration of pi estimation using TBB parallel_reduce

#include <iostream>
#include <tbb/tbb.h>
#include <tbb/parallel_for.h>

class MySeries{

    public:

        double my_sum;
        MySeries(MySeries &x, tbb::split): my_sum(0){
        }

        MySeries(): my_sum(0){
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
};

int main() {

    MySeries x;
    tbb::parallel_reduce(tbb::blocked_range<size_t>(0, 1000000000), x);
    std::cout << 4 * x.my_sum << std::endl;
    return 0;

}


/*
illustration of summing elements in a standard vector using parallel_reduce
*/

#include  <iostream>
#include <algorithm>
#include <numeric>
#include <tbb/tbb.h>

struct Sum {

    int value;

    Sum(): value(0){
    }

    Sum(Sum&s, tbb::split): value(0){
    }

    void operator()(const tbb::blocked_range<std::vector<int>::iterator>& r){
        value = std::accumulate(r.begin(), r.end(), value);
    }

    void join(Sum& rhs){
        value += rhs.value;
    }
};

int main(){

    // Create a standard vector of 100 elements.
    std::vector<int> a(100);
    // Fill all of the elements of the vector with 1.
    std::fill(a.begin(), a.end(), 1);
    // Create an instance of the summation structure.
    Sum sum;
    // Apply parallel_reduce.
    tbb::parallel_reduce(tbb::blocked_range<std::vector<int>::iterator>(
        a.begin(),
        a.end()),
        sum
    );
    std::cout << sum.value << std::endl;
    return 0;

}

#include <iostream>
#include <thread>
#include <vector>
#include <tbb/tbb.h>

//typedef tbb::concurrent_vector<long> my_long_vector;
typedef std::vector<long> my_long_vector;

void filler(my_long_vector& v, long offset, long n) {
    for (long i=offset; i<offset+n; ++i) {
        v.push_back(i);
    }
}

int main() {
    unsigned int const threads=8;
    unsigned int const push=10000;

    my_long_vector v;
    std::vector<std::thread> pool;

    for (size_t t=0; t<threads; ++t) {
        auto my_filler = std::bind(filler, std::ref(v), t*push, push);
        pool.push_back(std::thread(my_filler));
    }

    for (size_t t=0; t<threads; ++t)
        pool[t].join();

    size_t elements=0;
    for (auto el: v) {
        ++elements;
        std::cout << el << " ";
    }
    std::cout << std::endl;
    std::cout
        << "Counted "
        << elements
        << " elements (size is "
        << v.size()
        << ")"
        << std::endl;

    return 0;
}

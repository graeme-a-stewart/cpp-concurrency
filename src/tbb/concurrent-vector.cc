/*
illustration of thread safe vector container using TBB concurrent_vector
*/

#include <stdlib.h>
#include <vector>
#include <tbb/tbb.h>
#include <iostream>

// If you use the STL definition the code will not work as STL containers
// are not thread safe
// typedef std::vector<int> my_vector;
typedef tbb::concurrent_vector<int> my_vector_type;
my_vector_type my_vector;

void add_element(int i){
    my_vector.push_back(i);
}

int main(){
    const int size = 10000;
    tbb::parallel_for(0, size, 1, [=](int i){add_element(i);});

    size_t elements = 0;
    for (auto el: my_vector) {
      ++elements;
      std::cout << el << " ";
    }
    std::cout << std::endl;

    std::cout
        << "Counted "
        << elements
        << " elements (size is "
        << my_vector.size()
        << ")"
        << std::endl;

    return 0;
}


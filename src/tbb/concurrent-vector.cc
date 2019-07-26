/*
illustration of thread safe vector container using TBB concurrent_vector
*/

#include <stdlib.h>
#include <vector>
#include <tbb/tbb.h>
#include <iostream>

tbb::concurrent_vector<int> myList;

void add_element(int i){
    myList.push_back(i);
}

int main(){
    const int size = 10000;
    tbb::parallel_for(0, size, 1, [=](int i){add_element(i);});

    size_t elements = 0;
    for (auto el: myList) {
      ++elements;
      std::cout << el << " ";
    }
    std::cout << std::endl;

    std::cout
        << "Counted "
        << elements
        << " elements (size is "
        << myList.size()
        << ")"
        << std::endl;

    return 0;
}


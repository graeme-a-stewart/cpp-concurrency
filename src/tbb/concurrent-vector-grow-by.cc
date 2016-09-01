/*
illustration of thread safe vector container using TBB concurrent_vector
*/

#include <tbb/tbb.h>
#include <iostream>

tbb::concurrent_vector<int> myList;

void add_elements(int i){
    // Demonstrate how to use the grow_by(n) method to add n contiguous elements
    // to the vector.
    const size_t grow = 8;
    auto start_iterator = myList.grow_by(grow);
    for (size_t count=0; count<grow; ++count) {
        *start_iterator++ = i+count;
    }
}

int main(){
    const int size = 100;
    tbb::parallel_for(0, size, 1, [=](int i){add_elements(i);});

    size_t elements = 0;
    for (int el: myList) {
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

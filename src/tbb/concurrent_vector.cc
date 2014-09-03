/*
illustration of thread safe vector container using TBB concurrent_vector
*/

#include <stdlib.h>
#include <vector>
#include <tbb/tbb.h>

tbb::concurrent_vector<int> myList;

void add_element(int i){

    myList.push_back(i);

}

int main(){

    const int size = 10000;
    tbb::parallel_for(0, size, 1, [=](int i){add_element(i);});
    return 0;

}

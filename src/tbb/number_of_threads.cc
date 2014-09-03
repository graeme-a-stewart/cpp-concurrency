/*
illustration of control of number of threads and object-based TBB tasks (as
opposed to function-based tasks)
*/

#include <vector>
#include <iostream>
#include "tbb/parallel_for.h"
#include "tbb/task_scheduler_init.h"

struct mytask {

    mytask(size_t n): _n(n){
    }

    void operator()(){
        for (int i = 0; i < 1000000; ++i){}
        std::cerr << "[" << _n << "]";
    }

    size_t _n;

};

int main(int, char**){

    // automatic number of threads:
    //tbb::task_scheduler_init init;
    // explicit number of threads:
    tbb::task_scheduler_init init(
        tbb::task_scheduler_init::default_num_threads()
    );
    std::vector<mytask> tasks;
    for (int i=0; i < 1000;++i){
        tasks.push_back(mytask(i));
    }
    tbb::parallel_for(
        tbb::blocked_range<size_t>(0, tasks.size()),
        [&tasks](const tbb::blocked_range<size_t>& r){
        for (size_t i = r.begin(); i < r.end(); ++i) tasks[i]();
        }
    );
    std::cerr << std::endl;
    return 0;

}

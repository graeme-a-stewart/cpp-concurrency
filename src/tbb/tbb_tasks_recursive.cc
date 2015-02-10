#include "tbb/task_group.h"
#include <iostream>

int serialFib(int n){ 
  if (n< 2) {
    return n;
  }
  else {
    return serialFib(n-1)+serialFib(n-2);
  }
}

int Fib(int n) {
  if( n<2 ) { 
    return n;
  } else {
    int x, y;
    tbb::task_group g;
    g.run([&]{x=Fib(n-1);}); // spawn a task
    g.run([&]{y=Fib(n-2);}); // spawn another task
    g.wait();                // wait for both tasks to complete
    return x+y;
  }
}
int main (){ 
  std::cout << "serialFib: " << serialFib(40) << std::endl;
  std::cout << "parallelFib: " << Fib(40)<< std::endl;;
  return 0;
}

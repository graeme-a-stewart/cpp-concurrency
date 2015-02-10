#include "tbb/task_group.h"
#include <iostream>
#include <chrono>

int serialFib(int n){ 
  if (n< 2) {
    return n;
  }
  else {
    return serialFib(n-1)+serialFib(n-2);
  }
}

int Fib(int n) {
  if( n<20){
    return serialFib(n);
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

  std::chrono::time_point<std::chrono::system_clock> start, end;

  start = std::chrono::system_clock::now();
  int serial = serialFib(40);
  end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;

  std::cout << "serialFib result in " << elapsed_seconds.count() << " seconds: " << serial << std::endl;
  
  start = std::chrono::system_clock::now();
  int parallelFib = Fib(40);
  end = std::chrono::system_clock::now();
  elapsed_seconds = end-start;
  std::cout << "parallelFib result in " << elapsed_seconds.count() << " seconds: " << parallelFib<< std::endl;;
  return 0;
}

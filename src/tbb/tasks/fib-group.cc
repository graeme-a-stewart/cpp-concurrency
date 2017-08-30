// TBB task fibonnachi example (from https://software.intel.com/en-us/node/506102)

#include <iostream>
#include "tbb/tbb.h"

long Fib(long n) {
  if( n<2 ) {
    return n;
  } else {
    long x, y;
    tbb::task_group g;
    g.run([&]{x=Fib(n-1);}); // spawn a task
    g.run([&]{y=Fib(n-2);}); // spawn another task
    g.wait();                // wait for both tasks to complete
    return x+y;
  }
}

int main(int argc, char* argv[]) {
  long n = 18;
  if (argc>=2) {
    n = std::stoul(argv[1]);
  }

  long sum = Fib(n);

  std::cout << "Fibonnachi " << n << " is " << sum << std::endl;

  return 0;
}

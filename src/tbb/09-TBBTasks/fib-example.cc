// TBB task fibonnachi example (from https://software.intel.com/en-us/node/506102)

#include <iostream>
#include "tbb/tbb.h"

long SerialFib( long n ) {
  if( n<2 )
    return n;
  else
    return SerialFib(n-1)+SerialFib(n-2);
}

class FibTask: public tbb::task {
public:
  const long n;
  long* const sum;
  FibTask( long n_, long* sum_ ) :
    n(n_), sum(sum_)
  {}
  task* execute() {      // Overrides virtual function task::execute
    if( n<CutOff ) {
      *sum = SerialFib(n);
    } else {
      long x, y;
      FibTask& a = *new( allocate_child() ) FibTask(n-1,&x);
      FibTask& b = *new( allocate_child() ) FibTask(n-2,&y);
      // Set ref_count to 'two children plus one for the wait".
      set_ref_count(3);
      // Start b running.
      spawn( b );
      // Start a running and wait for all children (a and b).
      spawn_and_wait_for_all(a);
      // Do the sum
      *sum = x+y;
    }
    return NULL;
  }

private:
  const static long CutOff{10};
};


int main(int argc, char* argv[]) {
  long n = 24;
  long sum;

  if (argc>=2) {
    n = std::stoul(argv[1]);
  }

  FibTask& a = *new(tbb::task::allocate_root()) FibTask(n,&sum);
  tbb::task::spawn_root_and_wait(a);

  std::cout << "The answer was " << sum << std::endl;

  return 0;
}

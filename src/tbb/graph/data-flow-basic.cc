#include <iostream>

#include "tbb/tbb.h"
#include "tbb/flow_graph.h"

int main() {

  unsigned long sum = 0;
  tbb::flow::graph g;

  // Simple graph node that squares its argument; defined as a lambda
  // There is no state here so we can execute in parallel as many times as needed
  tbb::flow::function_node< unsigned long, unsigned long > squarer( g, tbb::flow::unlimited, [](const unsigned long &v) { 
      return v*v; 
    } );

  // Ditto for cubes
  tbb::flow::function_node< unsigned long, unsigned long > cuber( g, tbb::flow::unlimited, [](const unsigned long &v) { 
      return v*v*v; 
    } );

  // Add the input argument to the global sum - note here as their can be a
  // race condition we limit to a single execution
  tbb::flow::function_node< unsigned long, unsigned long > summer( g, 1, [&](const unsigned long &v ) -> unsigned long { 
      return sum += v; 
    } );

  // Connect the graph
  tbb::flow::make_edge( squarer, summer );
  tbb::flow::make_edge( cuber, summer );

  // Fire data into the graph
  for (int i=1; i<=1000; ++i) {
    squarer.try_put(i);
    cuber.try_put(i);
  }

  g.wait_for_all();

  std::cout << "Sum is " << sum << "\n";

  return 0;
}

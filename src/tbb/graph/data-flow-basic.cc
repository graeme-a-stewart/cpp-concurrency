#include <iostream>

#include "tbb/tbb.h"
#include "tbb/flow_graph.h"

int main() {

  int sum = 0;
  tbb::flow::graph g;

  tbb::flow::function_node< float, float > squarer( g, tbb::flow::unlimited, [](const float &v) { 
      return v*v; 
    } );
  tbb::flow::function_node< float, float > cuber( g, tbb::flow::unlimited, [](const float &v) { 
      return v*v*v; 
    } );
  tbb::flow::function_node< float, float > summer( g, 1, [&](const float &v ) -> float { 
      return sum += v; 
    } );
  tbb::flow::make_edge( squarer, summer );
  tbb::flow::make_edge( cuber, summer );

  for (int i=1; i<=10; ++i) {
    squarer.try_put(i);
    cuber.try_put(i);
  }

  g.wait_for_all();

  std::cout << "Sum is " << sum << "\n";

  return 0;
}

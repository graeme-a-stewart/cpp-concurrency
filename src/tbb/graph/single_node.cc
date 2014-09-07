#include <iostream>
#include <ctime>

#include "tbb/tbb.h"
#include "tbb/flow_graph.h"

int main() {
  tbb::flow::graph g;

  tbb::flow::function_node<int, int> n( g, tbb::flow::unlimited, []( int v ) -> int { 
      std::cout << v << std::endl;
      tbb::this_tbb_thread::sleep(tbb::tick_count::interval_t(1.0));
      std::cout << v << std::endl;
      return v;
    } );

  n.try_put(1);
  n.try_put(2);
  n.try_put(3);

  g.wait_for_all();

  return 0;
}

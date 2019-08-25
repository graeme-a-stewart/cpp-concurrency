#include <iostream>

#include "tbb/tbb.h"
#include "tbb/flow_graph.h"

int main() {
  tbb::flow::graph g;

  tbb::flow::function_node<int, int> n(g, tbb::flow::unlimited, []( int v ) -> int { 
      tbb::this_tbb_thread::sleep(tbb::tick_count::interval_t(0.1*v));
      std::cout << "node n " << v << std::endl;
      return v;
    } );

  tbb::flow::function_node<int, int> m(g, 1, []( int v ) -> int {
        v *= v;
	tbb::this_tbb_thread::sleep(tbb::tick_count::interval_t(0.5));
	std::cout << "node m " << v << std::endl;;
        return v;
    } );

  tbb::flow::make_edge(n, m);

  for (int i=0; i<10; ++i) {
    bool res = n.try_put(i);
    if (!res) {
      std::cout << "Failed to put " << i << std::endl;
    }
  }

  g.wait_for_all();

  return 0;
}

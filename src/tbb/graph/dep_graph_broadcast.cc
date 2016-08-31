// TBB Graph example showing how to use broadcast nodes
// to make 'tee' pipe connections in the DAG

#include <iostream>

#include "tbb/tbb.h"
#include "tbb/flow_graph.h"

typedef tbb::flow::function_node< tbb::flow::continue_msg, tbb::flow::continue_msg > node_t;
typedef tbb::flow::broadcast_node< tbb::flow::continue_msg > tee_t;
typedef const tbb::flow::continue_msg & msg_t;

int main() {
  tbb::flow::graph g;
  node_t A(g, 1, [](msg_t m){ std::cout << "A" << std::endl; return m; } );
  tee_t  B(g);
  node_t C(g, 1, [](msg_t m){ std::cout << "C" << std::endl; return m; } );
  node_t D(g, 1, [](msg_t m){ std::cout << "D" << std::endl; return m; } );
  tee_t  E(g);
  node_t F(g, 1, [](msg_t m){ std::cout << "F" << std::endl; return m; } );
  make_edge(A, B);
  make_edge(B, C);
  make_edge(B, D);
  make_edge(D, F);
  make_edge(A, E);
  make_edge(E, D);
  make_edge(E, F);
  A.try_put( tbb::flow::continue_msg() );
  g.wait_for_all();
  return 0;
}

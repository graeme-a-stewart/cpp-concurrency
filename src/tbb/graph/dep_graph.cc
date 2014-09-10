#include <iostream>

#include "tbb/tbb.h"
#include "tbb/flow_graph.h"

typedef tbb::flow::continue_node< tbb::flow::continue_msg > node_t;
typedef const tbb::flow::continue_msg & msg_t;

int main() {
  tbb::flow::graph g;
  node_t A(g, [](msg_t){ std::cout << "A" << std::endl; } );
  node_t B(g, [](msg_t){ std::cout << "B" << std::endl; } );
  node_t C(g, [](msg_t){ std::cout << "C" << std::endl; } );
  node_t D(g, [](msg_t){ std::cout << "D" << std::endl; } );
  node_t E(g, [](msg_t){ std::cout << "E" << std::endl; } );
  node_t F(g, [](msg_t){ std::cout << "F" << std::endl; } );
  make_edge(A, B);
  make_edge(B, C);
  make_edge(B, D);
  make_edge(A, E);
  make_edge(E, D);
  make_edge(E, F);
  A.try_put( tbb::flow::continue_msg() );
  g.wait_for_all();
  return 0;
}

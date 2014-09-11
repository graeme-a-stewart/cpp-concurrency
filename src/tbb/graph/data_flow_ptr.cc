#include <iostream>
#include <memory>

#include "tbb/tbb.h"
#include "tbb/flow_graph.h"

class src_body {
private:
  const int my_limit;
  int my_next_value;

public:
  src_body(int l) : 
    my_limit(l), my_next_value(1) {}

  bool operator()(int& v) {
    if ( my_next_value <= my_limit ) {
      v = my_next_value++;
      std::cout << "Source produces " << v << std::endl;
      return true;
    } else {
      return false;
    }
  }
};


int main() {

  int sum = 0;
  tbb::flow::graph g;

  tbb::flow::source_node< int > src( g, src_body(10), false );
  tbb::flow::function_node< int, int* > squarer( g, tbb::flow::unlimited, [](int v) {
      std::cout << "Sq got " << v << std::endl;
      int* v2 = new int(v*v);
      std::cout << "Sq said " << *v2 << std::endl;
      return v2; 
    } );
  tbb::flow::function_node< int, int* > cuber( g, tbb::flow::unlimited, [](int v) {
      std::cout << "Cu got " << v << std::endl;
      int* v3 = new int(v*v*v);
      std::cout << "Cu said " << *v3 << std::endl;
      return v3; 
    } );
  tbb::flow::function_node< int*, int > summer( g, 1, [&](int* v ) -> int {
      return sum+= *v;
    } );
  make_edge( src, squarer );
  make_edge( src, cuber );
  make_edge( squarer, summer );
  make_edge( cuber, summer );

  src.activate();
  g.wait_for_all();

  std::cout << "Sum is " << sum << "\n";

  return 0;
}

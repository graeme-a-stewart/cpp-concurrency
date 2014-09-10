#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <memory>

#include "strip_det.hpp"

#include "tbb/tbb.h"
#include "tbb/flow_graph.h"

using std::shared_ptr;
using std::make_shared;

using std::cout;
using std::endl;

class strip_loader {
private:
  std::ifstream* m_input_stream_p;
public:
  strip_loader(std::ifstream* ifs_p):
    m_input_stream_p(ifs_p) {};
  bool operator() (shared_ptr<det_strip>& ds_p) {
    cout << ds_p << endl;
    ds_p = make_shared<det_strip> ();
    bool rc = ds_p->load_strip(*m_input_stream_p);
    ds_p->dump_strip(cout);
    cout << rc << " " << ds_p << endl;
    return rc;
  }
};


int main() {
  tbb::flow::graph g;

  std::ifstream det_input("detector.txt", std::ofstream::out);

  tbb::flow::source_node<shared_ptr<det_strip>> loader(g, strip_loader(&det_input), false);
  tbb::flow::function_node<shared_ptr<det_strip>, shared_ptr<det_strip>> dumper(g, 1, [](const shared_ptr<det_strip>& ds_p) {
      cout << "dumper " << ds_p << endl;
      cout << "dumper " << ds_p->n_cells();
      ds_p->dump_strip(std::cout); 
      return ds_p;
    });

  tbb::flow::make_edge(loader, dumper);

  loader.activate();
  g.wait_for_all();

  return 0;
}

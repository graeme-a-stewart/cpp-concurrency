#include <iostream>
#include <fstream>

#include "tbb/tbb.h"
#include "tbb/flow_graph.h"

#include "boost/algorithm/string.hpp"

class file_cat {
private:
  std::ifstream* m_input_stream_p;
public:
  file_cat(std::ifstream* ifsp):
    m_input_stream_p(ifsp) {};

  bool operator() (std::string& msg) {
    *m_input_stream_p >> msg;
    if (m_input_stream_p->good())
      return true;
    return false;
  }
};

int main() {
  std::ifstream input("file_process.cc", std::ifstream::in);

  tbb::flow::graph g;

  tbb::flow::source_node<std::string> cat(g, file_cat(&input), false);
  tbb::flow::function_node<std::string, std::string> caps_printer(g, 1, [](std::string line) {
      boost::to_upper(line);
      std::cout << line << std::endl;
      return line;
    });

  tbb::flow::make_edge(cat, caps_printer);

  cat.activate();
  g.wait_for_all();

  input.close();

  return 0;
}

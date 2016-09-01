// Example showing how to use a source node to read an input file
// In this case we just use boost::to_upper to process the file
// into an ALL CAPS version
//
// file_process [INPUT_FILE]

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

int main(int argc, char* argv[]) {
  std::string input_file{"file_process.cc"};
  if (argc==2) {
    input_file = argv[1];
  } else if (argc > 2) {
	std::cerr << "Usage: file_process [INPUT_FILE]" << std::endl;
	exit(1);
  }

  std::ifstream input(input_file, std::ifstream::in);
  if (!input.good()) {
    std::cerr << "Problem with file " << input_file << " - aborting" << std::endl;
    exit(2);
  }

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

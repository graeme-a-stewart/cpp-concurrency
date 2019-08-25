#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

#include "stripdet.hpp"

int main() {
  std::ifstream ifs("fooble.txt", std::ifstream::in);

  det_strip test_strip{};
  for (bool read_ok=true; read_ok; ) {
    read_ok = test_strip.load_strip(ifs);
    if (read_ok)
      test_strip.dump_strip(std::cout);
  }

  ifs.close();

  return 0;
}

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

#include "strip_det.hpp"

int main() {
  std::ifstream ifs("detector.txt", std::ifstream::in);

  det_strip test_strip{};
  for (bool read_ok=true; read_ok; ) {
    read_ok = test_strip.load_strip(ifs);
    if (read_ok)
      test_strip.dump_strip(std::cout);
  }

  ifs.close();

  return 0;
}

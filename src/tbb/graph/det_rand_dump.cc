#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

#include "strip_det.hpp"

int main() {
  const unsigned int strips = 2;
  const unsigned int cells = 20;

  std::srand(std::time(nullptr));

  std::ofstream ofs("detector.txt", std::ofstream::out);

  det_strip test_strip{cells};
  for (size_t i=0; i<strips; ++i) {
    test_strip.fill_random();
    test_strip.dump_strip(ofs);
  }
  ofs.close();

  return 0;
}

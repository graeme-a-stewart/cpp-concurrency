#include <iostream>
#include <fstream>
#include <random>
#include <cmath>

#include "strip_det.hpp"

int main(int argc, char* argv[]) {
  unsigned int strips = 100;
  unsigned int cells = 200;

  if (argc==3) {
    strips = std::atoi(argv[1]);
    cells = std::atoi(argv[2]);
  } 

  std::default_random_engine generator;
  std::uniform_real_distribution<float> flat_dist(0.0, 1.0);

  // Pick a random place for the fooble
  long fooble_centre = (0.1 + flat_dist(generator) * 0.8) * strips;

  std::ofstream ofs("fooble.txt", std::ofstream::out);

  for (size_t i=0; i<strips; ++i) {
    float my_signal = 20.0 + 100.0 / (1.0 + std::pow(std::abs(long(i) - fooble_centre)/20.0, 2));
    float my_x = float(i) / strips;
    det_strip test_strip{cells, my_x};
    test_strip.fill_random(my_signal);
    test_strip.dump_strip(ofs);
  }
  ofs.close();

  return 0;
}

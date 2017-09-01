// Simple CPU burner to simulate workload
// calibrate the burn function and to
// evaluate overheads of the timed burn

#include <cmath>
#include <iostream>
#include <iomanip>
#include <chrono>

#include "tutorialutils.h"

void time_burn(unsigned long iterations) {
  auto start = std::chrono::system_clock::now();
  auto result = burn(iterations);
  auto stop = std::chrono::system_clock::now();
  auto duration = stop - start;
  std::cout << iterations << " took "
      << std::chrono::duration<float, std::milli> (duration).count() << "ms" << std::endl;
}

void time_burn_for(float ms_interval) {
  auto start = std::chrono::system_clock::now();
  burn_for(ms_interval);
  auto stop = std::chrono::system_clock::now();
  auto duration = stop - start;
  std::cout << std::setprecision(5) << std::scientific
      << "Timed burn of " << ms_interval << "ms "
      << " took " << std::chrono::duration<float, std::milli> (duration).count() << "ms" << std::endl;
}

int main(int argn, char *argv[]) {
  // Get some calibration data on how much time our burner uses
  unsigned long iterations = 100'000'000lu;
  while (iterations >= 10) {
    time_burn(iterations);
    iterations /= 10;
  }

  // Do some tests of timed burns, to see if there are
  // significant overheads
  float duration = 1000.0;
  while (duration > 0.00001) {
    time_burn_for(duration);
    duration /= 10.;
  }

  return 0;
}

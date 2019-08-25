#include <iostream>

#include "tbb/tbb_stddef.h"

int main() {
  std::cout << "TBB_INTERFACE_VERSION: " << TBB_INTERFACE_VERSION << std::endl;
  std::cout << "TBB_INTERFACE_VERSION_MAJOR: " << TBB_INTERFACE_VERSION_MAJOR << std::endl;
  std::cout << "TBB_COMPATIBLE_INTERFACE_VERSION: " << TBB_COMPATIBLE_INTERFACE_VERSION << std::endl;

  return 0;
}


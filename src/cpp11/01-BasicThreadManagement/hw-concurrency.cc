#include <iostream>
#include <thread>

int main() {
  std::cout << "Hardware concurrency is " << std::thread::hardware_concurrency() << std::endl;

  return 0;
}

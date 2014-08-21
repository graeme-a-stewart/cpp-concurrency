#include <iostream>
#include <thread>

void hello1() {
  std::cout << "hello, world (from thread 1)" << std::endl;
}

void hello2() {
  std::cout << "hello, world (from thread 2)" << std::endl;
}

int main() {
  std::thread t1(hello1);
  std::thread t2(hello2);

  t1.join();
  t2.join();

  return 0;
}


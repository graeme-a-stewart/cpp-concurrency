#include <thread>
#include <mutex>
#include <iostream>

#include "tbb/tbb.h"

#define SIZE 100
std::mutex kettle, tap;

void kettle_tap() {
  std::cout << "Locking kettle in " << std::this_thread::get_id() << std::endl;
  std::lock_guard<std::mutex> kettle_lock(kettle);
  std::cout << "Locked kettle in " << std::this_thread::get_id() << std::endl;
  
  std::cout << "Locking tap in " << std::this_thread::get_id() << std::endl;
  std::lock_guard<std::mutex> tap_lock(tap);
  std::cout << "Locked tap in " << std::this_thread::get_id() << std::endl;
  
  std::cout << "Filling kettle in " << std::this_thread::get_id() << std::endl;
}

void tap_kettle() {
  std::cout << "Locking tap in " << std::this_thread::get_id() << std::endl;
  std::lock_guard<std::mutex> tap_lock(tap);
  std::cout << "Locked tap in " << std::this_thread::get_id() << std::endl;
  
  std::cout << "Locking kettle in " << std::this_thread::get_id() << std::endl;
  std::lock_guard<std::mutex> kettle_lock(kettle);
  std::cout << "Locked kettle in " << std::this_thread::get_id() << std::endl;
  
  std::cout << "Filling kettle in " << std::this_thread::get_id() << std::endl;
}


int main() {
  tbb::parallel_for(0, SIZE, 1, [=](int i){
    kettle_tap();
    tap_kettle();
    });

  std::cout << "Work completed" << std::endl;

  return 0;
}

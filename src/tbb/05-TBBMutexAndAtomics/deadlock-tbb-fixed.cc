#include <thread>
#include <mutex>
#include <iostream>

#include "tbb/tbb.h"

#define SIZE 100
std::mutex kettle, tap;


void kettle_tap(){
  std::cout << "Locking kettle and tap in " << std::this_thread::get_id() << std::endl;

  std::scoped_lock lock_both{kettle, tap};

  std::cout << "Locked kettle and tap in " << std::this_thread::get_id() << std::endl;
  
  std::cout << "Filling kettle in " << std::this_thread::get_id() << std::endl;
}

void tap_kettle(){
  std::cout << "Locking tap and kettle in " << std::this_thread::get_id() << std::endl;

  std::scoped_lock lock_both{kettle, tap};

  std::cout << "Locked tap and kettle in " << std::this_thread::get_id() << std::endl;
  
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

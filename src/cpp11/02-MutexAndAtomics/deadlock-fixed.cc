#include <thread>
#include <mutex>
#include <iostream>

#define THREAD_POOL 80

std::mutex kettle, tap;

void kettle_tap() {
  std::cout << "Locking kettle and tap in "
      << std::this_thread::get_id() << std::endl;

  std::scoped_lock lock_both{kettle, tap};

  std::cout << "Locked kettle and tap in "
      << std::this_thread::get_id() << std::endl;
  
  std::cout << "Filling kettle in "
      << std::this_thread::get_id() << std::endl;
}

void tap_kettle() {
  std::cout << "Locking tap and kettle in " << std::this_thread::get_id() << std::endl;

  std::scoped_lock lock_both{kettle, tap};

  std::cout << "Locked tap and kettle in " << std::this_thread::get_id() << std::endl;
  
  std::cout << "Filling kettle in " << std::this_thread::get_id() << std::endl;
}

int main() {
  std::thread pool[THREAD_POOL];

  for (int t=0; t<THREAD_POOL; t+=2) {
    pool[t] = std::thread(kettle_tap);
    pool[t+1] = std::thread(tap_kettle);
  }

  for (int t=0; t<THREAD_POOL; ++t)
    pool[t].join();

  std::cout << "Threads are all joined" << std::endl;

  return 0;
}

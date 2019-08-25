// TBB task group, illustrating the cancel operation
// Note that the cancel will only cancel tasks that have not yet started

#include <chrono>
#include <thread>
#include <iostream>
#include "tbb/tbb.h"

#define DEPTH 30

int sleeper(int rest) {
  std::cout << "Thread " << std::this_thread::get_id() << " resting for " << rest << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(rest));
  std::cout << "Wake up after sleep of " << rest << std::endl;
  return 0;
}

int task_group_cancel() {
  tbb::task_group g;
  for (int i=1; i < DEPTH; ++i) {
    g.run([i]{sleeper(i);});
    std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Avoid mangling std output
  }
  std::this_thread::sleep_for(std::chrono::seconds(1));
  std::cout << "Cancelling tasks" << std::endl;
  g.cancel();
  g.wait(); // Wait for running tasks to finish or bad things will happen when g is destroyed

  return 0;
}


int main() {
  task_group_cancel();

  return 0;
}

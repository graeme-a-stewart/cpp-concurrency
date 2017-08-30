#include <string>
#include <iostream>
#include "tbb/tbb.h"

const std::string solution{"LLRRLLRL"};

int maze_try(std::string current_path) {
  if (current_path == solution) {
    std::cout << "I found the way out: " << current_path << std::endl;
    return 0;
  }

  if (current_path.size() == solution.size())
    return -1;

  tbb::task_group g;
  g.run([=]{maze_try(current_path+"L");});
  g.run([=]{maze_try(current_path+"R");});
  g.wait();
  return 1;
}

int main() {
  maze_try("");

  return 0;
}

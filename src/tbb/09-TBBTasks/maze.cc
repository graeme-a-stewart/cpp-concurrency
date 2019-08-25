// Simple maze solver demonstrating TBB task groups
#include <string>
#include <iostream>
#include "tbb/tbb.h"

const std::string solution{"LLRRLLRL"};

std::string maze_try(std::string current_path) {
  //std::cout << "Trying " << current_path << std::endl;

  if (current_path == solution) {
    return current_path;
  }

  if (current_path.size() == solution.size())
    return std::string{""};

  tbb::task_group g;
  std::string left, right;
  g.run([&]{left=maze_try(current_path+"L");});
  g.run([&]{right=maze_try(current_path+"R");});
  g.wait();

  if (left.size() > 0)
    return left;
  if (right.size() > 0)
    return right;

  return std::string{""};
}

int main() {
  auto answer = maze_try("");

  std::cout << "Escape route " << answer << std::endl;

  return 0;
}

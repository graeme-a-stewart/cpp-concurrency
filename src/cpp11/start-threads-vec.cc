// Extended simple example, showing how to use a vector
// to hold thread objects

#include <iostream>
#include <thread>
#include <vector>

void hello() {
    std::cout << "hello, world" << std::endl;
}

int main() {
    const int pool_size = 8;

    std::cout << "Starting " << pool_size << " threads" << std::endl;

    std::vector<std::thread> pool;
    for (int i=0; i<pool_size; ++i)
      pool.push_back(std::thread(hello));

    for (auto& th: pool)
      th.join();

    std::cout << "Threads are joined" << std::endl;

    return 0;
}


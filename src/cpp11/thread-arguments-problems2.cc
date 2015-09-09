#include <thread>
#include <vector>
#include <iostream>
#include <cmath>
#include <chrono>

#define SIZE 10000

void work_hard(std::vector<double>& d) {
    // Sleep here to artificially ensure that thread_spawn() has exited
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    double sum=0.0;
    for (auto& el: d) {
        sum += sin(cos(log(abs(el)+1.0)));
    }
    std::cout << "Hard work is done: " << sum << std::endl;
}

int thread_spawn() {
    std::vector<double> d;

    for (size_t i=0; i<SIZE; ++i)
        d.push_back(double(i)/SIZE);

    auto worker = std::thread(work_hard, std::ref(d));
    std::cout << "Worker thread spawned" << std::endl;
    worker.detach();

    // d is now going to go out of scope, probably
    // before the worker thread has finished with
    // catastrophic results
    return 0;
}

int main() {
    int code = thread_spawn();
    std::cout << "Spawner is done" << std::endl;
  
    // Sleep here to ensure the worker thread is not terminated early
    std::this_thread::sleep_for(std::chrono::seconds(1));

    return code;
}

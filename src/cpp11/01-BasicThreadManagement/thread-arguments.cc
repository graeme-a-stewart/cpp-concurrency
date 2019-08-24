// Simple example showing how to pass arguments to a thread
// function

#include <iostream>
#include <thread>

#define THREAD_POOL 50

void hello_n(int i) {
    std::cout << "hello, world (from thread number " << i << "; thread id" << std::this_thread::get_id() << ")" << std::endl;
}

int main() {
    std::cout << "Starting threads" << std::endl;

    std::thread pool[THREAD_POOL];

    for (int t=0; t<THREAD_POOL; ++t)
    	pool[t] = std::thread(hello_n, t);

    for (int t=0; t<THREAD_POOL; ++t)
        pool[t].join();

    std::cout << "Threads are joined" << std::endl;

    return 0;
}

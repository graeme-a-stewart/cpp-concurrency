#include <iostream>
#include <thread>
#include <mutex>

#define THREAD_POOL 5

std::mutex io_mtx;

void hello_n(int i) {
  std::lock_guard<std::mutex> io_lock (io_mtx);
  std::cout << "hello, world (from thread number " << i << "; thread id" << std::this_thread::get_id() << ")" << std::endl;
}

int main() {
  std::cout << "Starting threads (main thread id " << std::this_thread::get_id() << std::endl;

  std::thread pool[THREAD_POOL];

  for (int t=0; t<THREAD_POOL; ++t)
    pool[t] = std::thread(hello_n, t);

  for (int t=0; t<THREAD_POOL; ++t)
    pool[t].join();

  std::cout << "Threads are joined" << std::endl;

  return 0;
}


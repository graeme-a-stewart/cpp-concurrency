#include <future>
#include <iostream>
#include <random>
#include <thread>

double pi_estimator(long trials) {
  std::default_random_engine generator;
  std::uniform_real_distribution<double> distribution(0.0,1.0);

  double x, y;
  long success=0;
  for (long i=0; i<trials; ++i) {
    x = distribution(generator);
    y = distribution(generator);
    if (x*x + y*y <= 1.0)
      ++success;
  }

  return double(success)*4.0/trials;
}

int main() {
  unsigned int const thread_pool = std::thread::hardware_concurrency();
  long const trials_per_thread = 500000000;

  std::vector<std::future<double>> thread_results;

  std::cout << "Launching " << thread_pool << " calculations." << std::endl;

  for (int i=0; i<thread_pool; ++i)
    thread_results.push_back(std::async(std::launch::async,
					pi_estimator, trials_per_thread));

  std::cout << "Launched tasks" << std::endl;

  // I can work here
  double my_pi_sum = 0.0;
  for (auto& task_future: thread_results)
    my_pi_sum += task_future.get();

  double my_pi = my_pi_sum / thread_pool;

  std::cout << "Answer: " << my_pi << std::endl;

  return 0;
}

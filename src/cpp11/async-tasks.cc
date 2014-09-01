#include <future>
#include <iostream>
#include <random>

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
  long trials_per_thread = 100000000;

  std::future<double> thread_pi = std::async(std::launch::async,
					     pi_estimator, trials_per_thread);
  std::cout << "Launched async" << std::endl;

  // I can work here
  double my_pi = pi_estimator(trials_per_thread);
  std::cout << "My work done" << std::endl;
  
  double final_pi = (my_pi + thread_pi.get())/2.0;
  std::cout << "Averaged answer: " << final_pi << std::endl;

  return 0;
}

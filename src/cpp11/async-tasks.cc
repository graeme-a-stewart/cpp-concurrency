#include <future>
#include <iostream>
#include <random>
#include <exception>

// Launch a second thread for estimating pi via an async that
// returns a future

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
    std::future<double> thread_pi;

    try {
		thread_pi = std::async(
			std::launch::async, // Here we force an async lanuch in the background,
			                    // which can cause an exception to be thrown
			pi_estimator,
			trials_per_thread
		);
    }
    catch (const std::exception& e) {
    	std::cerr << "async threw an exception: " << e.what() << std::endl;
    	exit(1);
    }
    std::cout << "Launched async" << std::endl;

    // I can work here
    double my_pi = pi_estimator(trials_per_thread);
    std::cout << "My work done" << std::endl;
  
    double final_pi = (my_pi + thread_pi.get())/2.0;
    std::cout << "Averaged answer: " << final_pi << std::endl;

    return 0;
}

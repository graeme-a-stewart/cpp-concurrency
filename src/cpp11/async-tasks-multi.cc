#include <future>
#include <iostream>
#include <random>
#include <thread>
#include <string>

double pi_estimator(long trials) {
    // Ensure that each thread uses a separate seed.
    unsigned long seed = std::hash<std::thread::id>()(std::this_thread::get_id());
    std::default_random_engine generator(seed);
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

int main(int argc, char *argv[]) {
    unsigned int thread_pool = std::thread::hardware_concurrency();
    long const trials_per_thread = 100000000;

    std::vector<std::future<double>> thread_results;

    // If we are given a number on the command line, launch that number of tasks
    if (argc > 2) {
    	std::cerr << "Usage: async-tasks-multi [task_count]" << std::endl;
    	exit(1);
    } else if (argc == 2) {
    	thread_pool = std::stoi(argv[1]);
    }

    std::cout << "Launching " << thread_pool << " calculations." << std::endl;

    for (int i=0; i<thread_pool; ++i)
        thread_results.push_back(
            std::async(
                pi_estimator,
                trials_per_thread
            )
        );

    std::cout << "Launched tasks" << std::endl;

    // I can work here
    double my_pi_sum = 0.0;
    for (auto& task_future: thread_results)
        my_pi_sum += task_future.get();

    double my_pi = my_pi_sum / thread_pool;

    std::cout << "Answer: " << my_pi << std::endl;

    return 0;
}

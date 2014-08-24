#include <thread>
#include <vector>
#include <iostream>
#include <cmath>
#include <chrono>

#define SIZE 10000

void work_hard(std::vector<double>& d) {
	double sum=0.0;
	for (auto& item: d) {
		item = sin(cos(log(abs(item))));
		sum += item;
	}
	std::cout << "Worker is done: " << sum << std::endl;
}

int thread_spawn() {
	std::vector<double> d;
	
	for (size_t i=0; i<SIZE; ++i)
		d.push_back(double(i)/SIZE);
		
	auto worker = std::thread(work_hard, std::ref(d));
	std::cout << "Worker thread spawned" << std::endl;
	worker.join();
	
	// d is now going to go out of scope, probably
	// before the worker thread has finished with
	// catastrophic results
	return 0;
}

int main() {
	int code=thread_spawn();
	std::cout << "Spawner is done" << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(60));
	
	return code;
}

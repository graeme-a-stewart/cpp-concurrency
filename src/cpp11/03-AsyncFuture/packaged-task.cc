// Simple example of how to use a packaged task

#include <future>
#include <iostream>
#include <exception>
#include <cmath>

double the_frobincator(double d) {
	if (d <= 0.0) {
		throw std::runtime_error("illegal frobincation");
	}
	return std::log10(std::abs(d));
}

bool go_frobnicate(double d) {
	std::cout << "Started to frobincate " << d << std::endl;

	auto pt = std::packaged_task<double(double)>(the_frobincator);

	auto f = pt.get_future();

	// Usually one would now hand off the task to somewhere else to run
	// but here we just invoke it directly
	pt(d);

	try {
		auto my_result = f.get();
		std::cout << "Frobincation was " << my_result << std::endl;
	} catch(const std::exception& e) {
		std::cerr << "Frobincation failed: " << e.what() << std::endl;
		return false;
	}

	return true;
}

int main() {
	go_frobnicate(3.14159);
	go_frobnicate(0.0);

	return 0;
}

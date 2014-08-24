#include <thread>
#include <string>
#include <iostream>


class Tea {
private:
	std::string m_tea;
	
public:
	const char* tea() {
		return m_tea.c_str();
	}
	
	void set_tea(const char new_tea[]) {
		m_tea = new_tea;
	}
};

void tea_selector(Tea& tea, const char new_tea[]) {
	tea.set_tea(new_tea);
}

int main() {
	Tea my_cuppa;
	
	my_cuppa.set_tea("mint");
	
	std::cout << "Tea type: " << my_cuppa.tea() << std::endl;
	
	// Beware that copied reference does not refer to the 
	// original Tea instance
	auto threaded_tea = std::thread(tea_selector, my_cuppa, "black");
	threaded_tea.join();
	
	std::cout << "Updated tea type: " << my_cuppa.tea() << std::endl;

	return 0;
}

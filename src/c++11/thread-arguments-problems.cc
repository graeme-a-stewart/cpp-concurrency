#include <thread>
#include <string>
#include <iostream>
#include <functional>


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

  Tea():
    m_tea{""} {}

  Tea(const char first_cup[]):
    m_tea{first_cup} {}

  Tea(const Tea& other):
    m_tea{other.m_tea} {}

};

void tea_selector(Tea& tea, const char new_tea[]) {
  tea.set_tea(new_tea);
}

int main() {
  Tea my_cuppa("mint");
  
  std::cout << "Tea type: " << my_cuppa.tea() << std::endl;
	
  // Beware that copied reference does not refer to the 
  // original Tea instance
  auto threaded_tea = std::thread(std::bind(tea_selector, my_cuppa, "black"));
  threaded_tea.join();
  
  std::cout << "Updated tea type: " << my_cuppa.tea() << std::endl;

  return 0;
}

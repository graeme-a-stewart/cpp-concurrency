#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>

class det_cell {
private:
  bool m_alive;
  float m_sensor;
  float m_noise;

public:
  det_cell():
    m_alive{true}, m_sensor{0.0f}, m_noise{0.0f}
  {};

  det_cell(bool alive, float sensor, float noise):
    m_alive{alive}, m_sensor{sensor}, m_noise{noise}
  {};

  bool alive() {
    return m_alive;
  }

  float sensor() {
    return m_sensor;
  }

  float noise() {
    return m_noise;
  }

  // Is this cell ok?
  bool good_cell() {
    if (!m_alive || m_noise > m_sensor)
      return false;
    return true;
  }

  // If the cell is alive but the measurement is less than the noise
  // mark this cell as dead
  void suppress_noise() {
    if (!good_cell())
      m_alive = false;
  }

  // Fill the cell randomly (used for testing)
  void fill_random() {
    // % chance cell is dead
    if (std::rand()/float(RAND_MAX) < 0.02) {
      m_alive = false;
      m_sensor = 0.0f;
      m_noise = 0.0f;
    } else {
      m_alive = true;
      // noise is in range 0-10; sensor is in range 0-100
      m_noise = std::rand() * 10.0 / RAND_MAX;
      m_sensor = std::rand() * 10.0 / RAND_MAX;
    }
  }

  void dump_cell() {
    std::cout << m_alive << " " << m_sensor << " " << m_noise;
  }
};


class det_strip {
private:
  unsigned int m_n_cells;
  std::vector<det_cell> m_cells;

public:
  det_strip():
    m_n_cells{100}
  {
    m_cells.reserve(m_n_cells);
    for (size_t i=0; i<m_n_cells; ++i)
      m_cells.push_back(det_cell());
  };

  void fill_random() {
    for (auto& cell: m_cells)
      cell.fill_random();
  }

  void dump_strip() {
    for (auto& cell: m_cells) {
      cell.dump_cell();
      std::cout << std::endl;
    }
  } 
};  
    

int main() {
  std::srand(std::time(nullptr));

  det_strip test_strip;
  test_strip.fill_random();
  test_strip.dump_strip();

  return 0;
}

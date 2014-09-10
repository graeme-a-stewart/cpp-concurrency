#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>

#ifndef INCLUDE_STRIP_DET
#define INCLUDE_STRIP_DET 1

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
      m_noise = std::rand() * 10.0f / RAND_MAX;
      m_sensor = std::rand() * 100.0f / RAND_MAX;
    }
  }

  void dump_cell(std::ostream& ofs) {
    ofs << m_alive << " " << m_sensor << " " << m_noise;
  }

  bool load_cell(std::istream& ifs) {
    ifs >> m_alive >> m_sensor >> m_noise;
    if (!ifs.good())
      return false;
    return true;
  }

};


class det_strip {
private:
  unsigned int m_n_cells;
  std::vector<det_cell> m_cells;

public:
  det_strip():
    m_n_cells{0}
  {};

  det_strip(unsigned int n_cells):
    m_n_cells(n_cells)
  {
    m_cells.reserve(m_n_cells);
    for (size_t i=0; i<m_n_cells; ++i)
      m_cells.push_back(det_cell());
  };

  unsigned int n_cells() {
    return m_n_cells;
  }

  void fill_random() {
    for (auto& cell: m_cells)
      cell.fill_random();
  }

  void dump_strip(std::ostream& ofs) {
    ofs << m_n_cells << std::endl;
    for (auto& cell: m_cells) {
      cell.dump_cell(ofs);
      ofs << std::endl;
    }
  }

  bool load_strip(std::istream& ifs) {
    ifs >> m_n_cells;
    if (!ifs.good())
      return false;
    m_cells.resize(m_n_cells);
    for (auto& cell: m_cells) {
      if (!cell.load_cell(ifs))
	return false;
    }
    if (!ifs.good())
      return false;
    return true;
  }

};  
    

#endif  // INCLUDE_STRIP_DET

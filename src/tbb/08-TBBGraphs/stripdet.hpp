#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cmath>

#ifndef STRIP_DET_H
#define STRIP_DET_H 1

class det_cell {
private:
  bool m_alive;
  float m_sensor;
  float m_noise;

public:
  det_cell():
    det_cell(true, 0.0f, 0.0f)
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

  // Cell is ok if it was alive and low noise
  bool good_cell() {
    if (!m_alive || m_noise > m_sensor)
      return false;
    return true;
  }

  // Fill the cell randomly (used for testing and data generation)
  void fill_random(float signal) {
    // % chance cell is dead
    if (std::rand()/float(RAND_MAX) < 0.03) {
      m_alive = false;
      m_sensor = 0.0f;
      m_noise = 0.0f;
    } else {
      m_alive = true;
      // noise is in range 0-10; sensor is in range 0-SIGNAL
      m_noise = std::rand() * 10.0f / RAND_MAX;
      m_sensor = std::rand() * signal / RAND_MAX;
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
  size_t m_n_cells;
  float m_data_quality;
  float m_position;
  bool m_done_dq;
  std::vector<det_cell> m_cells;

public:
  det_strip():
    det_strip(0, 0.0f) {};

  det_strip(size_t n_cells, float position):
    m_n_cells{n_cells}, 
    m_data_quality{-1.0}, 
    m_position{position},
    m_done_dq{false}
  {
    m_cells.reserve(m_n_cells);
    for (size_t i=0; i<m_n_cells; ++i)
      m_cells.push_back(det_cell());
  };

  size_t n_cells() {
    return m_n_cells;
  }

  float position() {
    return m_position;
  }

  // Data quality can be cached in the m_data_quality member
  float data_quality() {
    if (m_done_dq)
      return m_data_quality;
    if (m_n_cells == 0)
      return -1.0f;
    size_t good_cells=0;
    m_done_dq = true;
    for (auto& cell: m_cells)
      good_cells += int(cell.good_cell());
    m_data_quality = float(good_cells)/m_n_cells;
    return m_data_quality;
  }

  // Calculate the signal in this strip, this is the
  // RMS of the signal in each cell which is alive
  float signal() {
    if (!m_done_dq)
      return 0.0f;
    float sq_sum = 0.0;
    size_t live_cells = 0;
    for (auto& cell: m_cells) {
      if (cell.good_cell()) {
	sq_sum += cell.sensor() * cell.sensor();
	++live_cells;
      }
    }
    if (!live_cells)
      return 0.0f;
    return std::sqrt(sq_sum/live_cells);
  } 

  // This is the silly fooble detction calculation
  bool fooble() {
    if (!m_done_dq) return false;
    float fooble_trigger = 0.0;
    for (auto& cell : m_cells) {
      if (cell.good_cell() && cell.sensor() > cell.noise() * 3.0) {
        float answer = cell.sensor() - cell.noise();
        // Time wasting ;-)
        for (int i = 0; i < 1000; ++i) answer += log(pow(answer + 1.0, 2.5));
        fooble_trigger += answer;
      }
    }
    if (fooble_trigger > 3.0e+6) return true;
    return false;
  }

  void fill_random(float signal) {
    for (auto& cell : m_cells) cell.fill_random(signal);
  }

  void dump_strip(std::ostream& ofs) {
    ofs << m_n_cells << " " << m_position << std::endl;
    for (auto& cell: m_cells) {
      cell.dump_cell(ofs);
      ofs << std::endl;
    }
  }

  bool load_strip(std::istream& ifs) {
    ifs >> m_n_cells >> m_position;
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
    

#endif  // STRIP_DET_H

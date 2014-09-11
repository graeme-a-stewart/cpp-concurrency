#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <memory>

#include "strip_det.hpp"

#include "tbb/tbb.h"
#include "tbb/flow_graph.h"

using std::shared_ptr;
using std::make_shared;

using std::cout;
using std::endl;

class strip_loader {
private:
  size_t& m_strip_counter;
  std::ifstream* m_input_stream_p;
public:
  strip_loader(std::ifstream* ifs_p, size_t& strip_counter):
    m_strip_counter{strip_counter}, m_input_stream_p(ifs_p) {};
  bool operator() (shared_ptr<det_strip>& ds_p) {
    //cout << ds_p << endl;
    ds_p = make_shared<det_strip> ();
    bool rc = ds_p->load_strip(*m_input_stream_p);
    //ds_p->dump_strip(cout);
    //cout << rc << " " << ds_p << endl;
    if (rc)
      ++m_strip_counter;
    return rc;
  }

  size_t strip_counter() {
    return m_strip_counter;
  }
};

class fooble_counter {
private:
  size_t& m_fooble_counter;
public:
  fooble_counter(size_t& fooble_counter):
    m_fooble_counter{fooble_counter} {};

  bool operator() (bool fbl) {
    if (fbl)
      ++m_fooble_counter;
    return true;
  }
};

class dq_hist {
private:
  float m_start;
  float m_end;
  size_t m_bins;
  std::vector<size_t> m_counts;
  std::vector<float> m_dq;
  std::vector<size_t> m_signal;

  size_t get_bin(float x) {
    size_t bin = ((x - m_start) / (m_end - m_start)) * m_bins;
    if (bin < 0)
      return 0;
    if (bin >= m_bins)
      return m_bins-1;
    return bin;
  }	     

public:
  dq_hist(float start, float end, size_t bins):
    m_start{start}, m_end{end}, m_bins{bins}
  {
    m_counts.assign(m_bins, 0);
    m_dq.assign(m_bins, 0.0);
    m_signal.assign(m_bins, 0.0);
  };

  tbb::flow::continue_msg operator() (const shared_ptr<det_strip>& ds_p) {
    fill(ds_p->position(), ds_p->data_quality(), ds_p->signal());
    return tbb::flow::continue_msg{};
  }

  void fill(float x, float dq, float signal) {
    size_t bin = get_bin(x);
    ++m_counts[bin];
    m_dq[bin] += dq;
    m_signal[bin] += signal;
  }

  void write_hist(std::ostream& ofs) {
    ofs << "Bin DQ Signal" << endl;
    for (size_t i=0; i<m_bins; ++i) {
      ofs << i << " ";
      if (m_counts[i] == 0) {
	ofs << 0.0 << " " << 0.0 << endl;
      } else {
	ofs << m_dq[i]/m_counts[i] << " " << m_signal[i]/m_counts[i] << endl;
      }
    }
  }
};


int main() {
  tbb::flow::graph g;

  std::ifstream det_input("fooble.txt", std::ofstream::out);

  size_t total_strips = 0;
  size_t counted_foobles = 0;
  dq_hist my_dq(0.0, 1.0, 10);

  tbb::flow::source_node<shared_ptr<det_strip>> loader(g, strip_loader(&det_input, total_strips), false);
  tbb::flow::function_node<shared_ptr<det_strip>, shared_ptr<det_strip>> calc_dq(g, tbb::flow::unlimited, [](const shared_ptr<det_strip>& ds_p) {
      float dq = ds_p->data_quality();
      return ds_p;
    });
  tbb::flow::function_node<shared_ptr<det_strip>, shared_ptr<det_strip>> get_signal(g, tbb::flow::unlimited, [](const shared_ptr<det_strip>& ds_p) {
      float signal = ds_p->signal();
      return ds_p;
    });
  tbb::flow::function_node<shared_ptr<det_strip>, bool> get_fooble(g, tbb::flow::unlimited, [](const shared_ptr<det_strip>& ds_p) {
      bool saw_fooble = ds_p->fooble();
      if (saw_fooble && ds_p->data_quality() > 0.9) {
	cout << "fooble " << saw_fooble << " at " << ds_p->position() << endl;
	return true;
      }
      return false;
    });
  tbb::flow::function_node<bool, bool> count_fooble(g, 1, fooble_counter{counted_foobles});
  tbb::flow::function_node<shared_ptr<det_strip>> dq_hist(g, 1, std::ref(my_dq));

  // Test node - don't connect this node in production ;-)
  tbb::flow::function_node<shared_ptr<det_strip>, shared_ptr<det_strip>> dumper(g, 1, [](const shared_ptr<det_strip>& ds_p) {
      ds_p->dump_strip(std::cout); 
      return ds_p;
    });

  tbb::flow::make_edge(loader, calc_dq);
  tbb::flow::make_edge(calc_dq, get_signal);
  tbb::flow::make_edge(calc_dq, get_fooble);
  tbb::flow::make_edge(get_fooble, count_fooble);
  tbb::flow::make_edge(get_signal, dq_hist);

  loader.activate();
  g.wait_for_all();

  cout << "---------" << endl;
  my_dq.write_hist(cout);
  cout << "---------" << endl;
  cout << "Foobles counted: " << counted_foobles << " from " << total_strips << endl;
  if (float(counted_foobles) / total_strips > 0.2) {
    cout << "WE HAVE FOUND A FOOBLE!" << endl;
  }
  cout << "---------" << endl;

  return 0;
}

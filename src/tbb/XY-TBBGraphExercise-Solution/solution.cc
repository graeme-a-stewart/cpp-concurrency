// This shows one possible solution to the TBB Graph
// exercise
//
// It is correct, but there are further optimisations
// that could be done - left as an exercise for group
// discussion and for the reader to implent

#include <iostream>
#include <vector>
#include <tbb/tbb.h>


#include "fdet.hpp"

// Define our detector data vector type here
// This allows us to see what might go wrong is we were to use an std::vector
// (could it be corrected?)
using f_det_vec = tbb::concurrent_vector<f_det>;

// Read input data from a file
class frame_loader {
private:
  size_t m_frame_counter;
  std::ifstream& m_input_stream_p;
public:
  frame_loader(std::ifstream &ifs_p):
    m_frame_counter{0}, m_input_stream_p(ifs_p) {};

  bool operator() (f_det& fdet) {
    int read_err;
    read_err = fdet.read(m_input_stream_p);
    if (read_err) {
        return false;
    }
    ++m_frame_counter;
    return true;
  }

  size_t frame_counter() {
    return m_frame_counter;
  }
};

// Aggregate frame data into the global data vector, return the index
// of the data item added
class add_frame_data {
private:
    f_det_vec& m_fdet_data;

public:
    add_frame_data(f_det_vec& fdet_data):
        m_fdet_data{fdet_data} {};

    size_t operator()(f_det fdet) {
        m_fdet_data.push_back(fdet);
        std::cout << "Added new frame, average " << fdet.average() << std::endl;
        return m_fdet_data.size();
    }
};

// Subtract the pedastal values from a frame
class subtract_pedastal {
private:
    f_det_vec& m_fdet_data;

public:
    subtract_pedastal(f_det_vec& fdet_data):
        m_fdet_data{fdet_data} {};

    size_t operator()(size_t t) {
        tbb::parallel_for(tbb::blocked_range2d<size_t>(0, DETSIZE, 0, DETSIZE),
        [&, t](tbb::blocked_range2d<size_t>& r){
            for (size_t x=r.rows().begin(); x!=r.rows().end(); ++x) {
                for (size_t y=r.cols().begin(); y!=r.cols().end(); ++y) {
                    m_fdet_data[t].cells[x][y] -= pedastal(x, y);
                }
            }
        });
        return t;
    }
};

int main(int argn, char* argv[]) {
    if (argn != 2) {
        std::cerr << "Usage: solution INPUT_FILE" << std::endl;
        return 1;
    }

    std::ifstream det_in(argv[1], std::ios::binary);
    if (!det_in.good()) {
        std::cerr << "Problem opening imput file" << std::endl;
        return 2;
    }

    // Setup a big vector where we will add all the data
    // Assume in this case it fits in memory!
    f_det_vec fdet_data;
    frame_loader data_loader(det_in);
    add_frame_data frame_aggregator(fdet_data);
    subtract_pedastal sub_pedastal(fdet_data);

    tbb::flow::graph data_process;
    tbb::flow::source_node<f_det> loader(data_process, data_loader, false);
    tbb::flow::function_node<f_det, size_t> aggregate(data_process, 1, frame_aggregator);
    tbb::flow::function_node<size_t, size_t> pedastal(data_process, tbb::flow::unlimited, frame_aggregator);

    tbb::flow::make_edge(loader, aggregate);
    tbb::flow::make_edge(aggregate, pedastal);

    loader.activate();
    data_process.wait_for_all();

    return 0;
}

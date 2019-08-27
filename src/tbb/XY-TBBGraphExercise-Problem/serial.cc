// This gives a serial solution to the fooble
// detection problem
//
// It is implemeneted here as a helper and reference
// for a parallel solution approach, however, don't 
// just naievely parallelise the code, do make sure
// you think about what a suitable parallel solution
// would be...

#include <algorithm>
#include <iostream>
#include <vector>
#include <array>

#include "fdet.hpp"

#define DEBUG 1

// Define our detector data vector type here
// In this solution we're going to be a bit lazy and
// assume we can hold all of the data in memory
using f_det_vec = std::vector<fdet::f_det>;

// This is the "datatype" we shall use to gather the
// signals, which will be an array of vectors, the
// [x][y] array corresponds to the cell coordinate
// and the value is a vector of frame indexes
// where that cell saw a signal.
// In this case it's really more convenient to use a struct
struct det_signal {
    std::array<std::vector<size_t>, fdet::detsize> count[fdet::detsize];
};

// Foobles are detected as a struct with t, x, y and duration
struct fooble {
    size_t x, y, t, d;

    fooble(size_t _x, size_t _y, size_t _t, size_t _d): 
        x{_x}, y{_y}, t{_t}, d{_d} {};
};

// Read input data from a file
int loader(std::ifstream& in_fp, fdet::f_det& frame) {
    int rerr = frame.read(in_fp);
    return rerr;
}


// Subtract the pedastal values from a frame
int pedastal_subtract(fdet::f_det& frame) {
    for (size_t x=0; x<fdet::detsize; ++x) {
        for (size_t y=0; y<fdet::detsize; ++y) {
            frame.cells[x][y] -= fdet::pedastal(x, y);
        }
    }
    return 0;
}

// Data quality, apply mask to bad cells
int mask_bad_cells(fdet::f_det& frame) {
    // Loop over cells and set any bad cells to -1.0
    for (size_t x=0; x<fdet::detsize; ++x) {
        for (size_t y=0; y<fdet::detsize; ++y) {
            if (!fdet::cell_mask(x, y)) frame.cells[x][y] = -1.0f;
        }
    }
    return 0;
}


// Signal search in a frame, looking for 3x3 clusters
// which are above our threshold
int signal_search(det_signal& signals, size_t frame_no, fdet::f_det& frame) {
    for (size_t x=0; x<fdet::detsize; ++x) {
        for (size_t y=0; y<fdet::detsize; ++y) {
            float sum = 0.0f;
            int count = 0;
            for (auto dx=x-1; dx<x+2; ++dx) {
                for (auto dy=y-1; dy<y+2; ++dy) {
                    if (dx>=0 && dx<fdet::detsize && dy>=0 && dy<fdet::detsize && 
                        frame.cells[dx][dy]>0.0) {
                        ++count;
                        sum += frame.cells[dx][dy];
                    }
                }
            }
            if (sum > count*fdet::signal_threshold) {
                signals.count[x][y].push_back(frame_no);
                if (DEBUG) {
                    std::cout << "Signal " << sum/count << " " << count 
                        << " in frame " << frame_no << std::endl;
                }
            }
        }
    }
    return 0;
}


// Fooble detector code
std::pair<int, int> detect_fooble_in_cell(std::vector<size_t> cell_signal) {
    // Give up on hopeless cases...
    if (cell_signal.size() < fdet::fooble_det_time) return std::pair<int, int>(-1, -1);

    if (DEBUG) {
        std::cout << "Attempting fooble detection on " << cell_signal.size() 
            << " signals" << std::endl;
    }

    // Ensure that signals are time ordered
    std::sort(cell_signal.begin(), cell_signal.end(), std::less<size_t>());
    if (DEBUG) {
        for (auto t: cell_signal) {
            std::cout << t << " ";
        }
        std::cout << std::endl;
    }
    int test_value = -1;
    int last_value = -1;
    int duration = 1;
    int detection = -1;
    int detection_duration = -1;
    for (size_t i=0; i<cell_signal.size(); ++i) {
        if (test_value<0) {
            test_value = cell_signal[i];
            last_value = cell_signal[i];
            duration = 1;
        } else if (cell_signal[i]==last_value+1) {
            last_value=cell_signal[i];
            ++duration;
        } else {
            if (duration >= fdet::fooble_det_time) {
                detection = test_value;
                detection_duration = duration;
            }
            test_value = cell_signal[i];
            last_value = cell_signal[i];
            duration = 1;
        }
    }
    // Have to handle properly the end of the window
    if (duration >= fdet::fooble_det_time) {
        detection = test_value;
        detection_duration = duration; 
    }

    return std::pair<int, int>(detection, detection_duration);
}


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
    det_signal fdet_signal;


    while(det_in.good()) {
        // Try to load the next data frame
        fdet::f_det new_frame;
        auto rerr = loader(det_in, new_frame);
        if (!rerr) {
            // Do pedastal subtraction
            pedastal_subtract(new_frame);

            // Mask bad cells
            mask_bad_cells(new_frame);

            // Now stack the prepped data
            fdet_data.push_back(new_frame);
        }
    }

    // After data has been loaded, search for signals
    // in each frame
    for (size_t f=0; f<fdet_data.size(); ++f) {
        signal_search(fdet_signal, f, fdet_data[f]);
    }

    // To look for foobles we need to look at the clusters
    // through time
    std::vector<fooble> detected_foobles;
    for (size_t x=0; x<fdet::detsize; ++x) {
        for (size_t y=0; y<fdet::detsize; ++y) {
            std::pair<int, int> detect = detect_fooble_in_cell(fdet_signal.count[x][y]);
            if (detect.first >= 0) {
                detected_foobles.push_back(
                    fooble(x, y, detect.first, detect.second)
                );
            }
        }
    }

    // Finally...
    std::cout << "Fooble detection report" << std::endl 
              << "-----------------------" << std::endl;
    std::cout << detected_foobles.size() << " were found" << std::endl;
    for (auto f: detected_foobles) {
        std::cout << "Frame " << f.t << ", duration " << f.d <<
            " at (" << f.x << ", " << f.y << ")" << std::endl;
    }

    return 0;
}

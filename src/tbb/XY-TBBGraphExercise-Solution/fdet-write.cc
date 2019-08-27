// Code to write fdet data sample
//
// This can be used to generate test data on which
// the fooble detector code can be tested

#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <mutex>
#include <tbb/tbb.h>

#include "fdet.hpp"

void averages(std::vector<fdet::f_det> &fdet_data) {
    std::cout << "Calculating averages of " << fdet_data.size() << " frames" << std::endl;
    for(size_t t=0; t<fdet_data.size(); ++t) {
        std::cout << "Timeframe " << t << ": average values "
            << fdet_data[t].average() << " " << fdet_data[t].s_average()
            << std::endl;
    }
}

void signal_place(fdet::f_det &frame, size_t x, size_t y) {
    for (size_t dx=x-1; dx<x+2; ++dx) {
        for (size_t dy=y-1; dy<y+2; ++dy) {
            if (dx>=0 && dx<fdet::detsize && dy>=0 && dy<fdet::detsize) {
                    frame.cells[dx][dy] += 200.0f;
                }
        }
    }
    // Centre boost
    frame.cells[x][y] += 50.0f;
}

int main(int argn, char* argv[]) {
    int frames{50};
    int foobles{1};
    unsigned long base_seed{0};
    std::string outfile{"input-data.bin"};

    if (argn==5) {
        frames = std::stoi(argv[1]);
        foobles = std::stoi(argv[2]);
        base_seed = std::stoul(argv[3]);
        outfile = std::string(argv[4]);
    } else if (argn != 1) {
        std::cout << "Usage: fdet-write TIME_FRAMES FOOBLES RANDOM_SEED OUTPUT_FILE" << std::endl;
        return 1;
    }

    std::vector<fdet::f_det> fdet_data;
    fdet_data.reserve(frames);
    for (size_t t=0; t<frames; ++t) {
        fdet_data.push_back(fdet::f_det());
    }


    // Start by filling each readout frame with noise
    // Note that std::random generators are not generally thread safe
    // and certainly not reproducible when threads are retrieving
    // values in a random order, so the strategy is to use a 
    // different seed within each frame, and access in a consistent
    // order (given that frames >> cores this is pretty much ok)
    std::cout << "Filling detector with noise" << std::endl;
    tbb::parallel_for(0, frames, 1, [&, base_seed](int t){
        std::mt19937 generator;
        generator.seed(base_seed+t);
        std::normal_distribution<float> noise{10,4};
        fdet_data[t].timestamp = float(t);
        for (size_t i=0; i<fdet::detsize; ++i) {
            for (size_t j=0; j < fdet::detsize; ++j) {
                fdet_data[t].cells[i][j] = noise(generator);
            }
        }
    });
    averages(fdet_data);

    // Now add pedastal values to each cell
    // This works very nicely as a 3d parallel_for
    // To avoid repeating the loop we also blow up
    // hot cell values here
    std::cout << "Adding pedastal values and hot cells" << std::endl;
    tbb::parallel_for(tbb::blocked_range3d<size_t>(0, frames, 0, fdet::detsize, 0, fdet::detsize), 
        [&](tbb::blocked_range3d<size_t>& r){
            for (size_t t=r.pages().begin(); t!=r.pages().end(); ++t) {
                for (size_t x=r.rows().begin(); x!=r.rows().end(); ++x) {
                    for (size_t y=r.cols().begin(); y!=r.cols().end(); ++y) {
                        fdet_data[t].cells[x][y] += fdet::pedastal(x, y);
                        if (fdet::cell_mask(x, y) == false) fdet_data[t].cells[x][y] += 6666.0f;
                    }
                }
            }
        }
    );
    averages(fdet_data);


    // Now add our foobles
    // We decide at random where the centre of a fooble will be and what the 
    // starting timeframe is, then we decide on a signal time length and
    // boost the cells here with the fooble signal
    std::cout << "Placing " << foobles << " foobles" << std::endl;
    std::mt19937 generator;
    generator.seed(base_seed+666000);
    std::uniform_real_distribution<float> rdist(0.0, 1.0);
    for (int i=0; i<foobles; ++i) {
        size_t fooble_x = 1 + (fdet::detsize-1) * rdist(generator);
        size_t fooble_y = 1 + (fdet::detsize-1) * rdist(generator);
        size_t fooble_t = (frames-10) * rdist(generator);
        size_t fooble_duration = 5 + 5.0 * rdist(generator);
        std::cout << "Placing fooble at (" << fooble_t << ", " << fooble_x <<
            ", " << fooble_y << ")"  << 
            "for " << fooble_duration << " frames" << std::endl;
        for (size_t t=fooble_t; t<fooble_t+fooble_duration; ++t) {
            signal_place(fdet_data[t], fooble_x, fooble_y);
        }

    }
    averages(fdet_data);

    // Now add a few spurious signals to each timeframe
    for (auto& f: fdet_data) {
        std::poisson_distribution<int> p(8);
        int count = p(generator);
        for (int i=0; i<count; ++i) {
            size_t noise_x = 1 + (fdet::detsize-1) * rdist(generator);
            size_t noise_y = 1 + (fdet::detsize-1) * rdist(generator);
            signal_place(f, noise_x, noise_y);
        }
    }


    // Now write out the detector
    std::cout << "Writing out detector data to " << outfile <<
        " (" << frames << " frames)" << std::endl;
    std::ofstream det_out(outfile, std::ios::out | std::ios::binary);
    int write_err{0};
    for (size_t t=0; t<frames; ++t) {
        write_err = fdet_data[t].write(det_out);
        if (write_err) {
            std::cerr << "Error writing frame " << t << std::endl;
        }
    }

    std::cout << "End of writer" << std::endl;

    return 0;
}
 
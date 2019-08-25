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

void averages(std::vector<f_det> &fdet_data) {
    std::cout << "Calculating averages of " << fdet_data.size() << " frames" << std::endl;
    for(size_t t=0; t<fdet_data.size(); ++t) {
        std::cout << "Timeframe " << t << ": average values "
            << fdet_data[t].average() << " " << fdet_data[t].s_average()
            << std::endl;
    }
}

int main(int argn, char* argv[]) {
    int frames{20};
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

    std::vector<f_det> fdet_data;
    fdet_data.reserve(frames);
    for (size_t t=0; t<frames; ++t) {
        fdet_data.push_back(f_det());
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
        for (size_t i=0; i<DETSIZE; ++i) {
            for (size_t j=0; j < DETSIZE; ++j) {
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
    tbb::parallel_for(tbb::blocked_range3d<size_t>(0, frames, 0, DETSIZE, 0, DETSIZE), 
        [&](tbb::blocked_range3d<size_t>& r){
            for (size_t t=r.pages().begin(); t!=r.pages().end(); ++t) {
                for (size_t x=r.rows().begin(); x!=r.rows().end(); ++x) {
                    for (size_t y=r.cols().begin(); y!=r.cols().end(); ++y) {
                        fdet_data[t].cells[x][y] += pedastal(x, y);
                        if (cell_mask(x, y) == false) fdet_data[t].cells[x][y] += 6666.0f;
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
        size_t fooble_x = 1 + (DETSIZE-1) * rdist(generator);
        size_t fooble_y = 1 + (DETSIZE-1) * rdist(generator);
        size_t fooble_t = (frames-5) * rdist(generator);
        size_t fooble_duration = 5;
        std::cout << "Placing fooble at (" << fooble_t << ", " << fooble_x <<
            ", " << fooble_y << ")"  << std::endl;
        for (size_t t=fooble_t; t<fooble_t+5; ++t) {
            for (size_t x=fooble_x-1; x<fooble_x+2; ++x) {
                for (size_t y=fooble_y-1; y<fooble_y+2; ++y) {
                    fdet_data[t].cells[x][y] += 200.0f + 20.0f*rdist(generator);
                }
            }
            // Centre boost
            fdet_data[t].cells[fooble_x][fooble_y] += 150.0f + 30.0f*rdist(generator);
        }

    }
    averages(fdet_data);


    // Now write out the detector
    std::cout << "Writing out detector data to " << outfile <<
        " (" << frames << " frames)" << std::endl;
    std::ofstream det_out("input-data.bin", std::ios::out | std::ios::binary);
    for (size_t t=0; t<frames; ++t) {
        det_out.write(reinterpret_cast<char*>(&fdet_data[t].timestamp), sizeof(float));
        det_out.write(reinterpret_cast<char*>(&fdet_data[t].cells), sizeof(float)*DETSIZE*DETSIZE);
    }

    std::cout << "End of writer" << std::endl;

    return 0;
}
 
// Example of reader of fdet data

#include <iostream>
#include <fstream>
#include <vector>

#include "fdet.hpp"

int main(int argn, char* argv[]) {
    if (argn != 2) {
        std::cerr << "Usage: fdet-read INPUT_FILE" << std::endl;
        return 1;
    }

    std::vector<fdet::f_det> fdet_data;

    std::ifstream det_in(argv[1], std::ios::binary);
    size_t frame_counter{0};
    int read_error{0};
    while(det_in.good()) {
        fdet::f_det fdet;
        read_error = fdet.read(det_in);
        if (!read_error) {
            std::cout << "Read frame " << frame_counter << ": Average, " << fdet.average() << std::endl;
            fdet_data.push_back(fdet);
            ++frame_counter;
        }
        if (frame_counter==12) fdet.dump_csv("test.csv");
    }

    return 0;
}

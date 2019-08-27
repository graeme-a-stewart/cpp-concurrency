// Example of reader of fdet data

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include "fdet.hpp"

int main(int argn, char* argv[]) {
    if (argn < 2 || argn > 3) {
        std::cerr << "Usage: fdet-read INPUT_FILE [DUMP_FRAME]" << std::endl;
        return 1;
    }

    bool do_dump = false;
    size_t dump_frame = 0;
    if (argn == 3) {
        do_dump = true;
        dump_frame = std::stoul(argv[2]);
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
            if (do_dump && frame_counter==dump_frame) {
                std::ostringstream fname;
                fname << "dump-" << dump_frame << ".csv" << std::ends;
                std::string s = fname.str();
                fdet.dump_csv(s.c_str());
                std::cout << "Dumped frame " << dump_frame << std::endl;
            }
            ++frame_counter;
        }
    }

    return 0;
}

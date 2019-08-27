// Header file for fooble detector
//
// Defines the simple class used to represent detector data

#include <array>
#include <fstream>

namespace fdet {

    // Define a few constants here that we shall use in the 
    // exercise
    const static size_t detsize = 100;
    const static float signal_threshold=200.0f;
    const static int fooble_det_time=5;

    // Structure for holding detector data
    struct f_det {
        // This is the core of the detector representaion,
        // with one float for the timestamp and then a simple
        // array of data for the detector cells (x,y) layout
        float timestamp;
        std::array<float, detsize> cells[detsize];

        // Constructors
        f_det() {};
        f_det(float t) : timestamp{t} {};
        f_det(float t, float v);

        // Utility functions for cross checking processing steps
        // (average uses TBB, s_average is a serial version)
        float average();
        float s_average();

        // Read/write detector frame data from a suitable file handle
        // N.B. set std::ios::binary option!
        int read(std::ifstream &input_fp);
        int write(std::ofstream &output_fp);

        // Export detector data in CSV format (then easy to import
        // into python and plot)
        int dump_csv(const char fname[]);
    };


    //// Data preparation functions
    // Pedastal function, returns the pedastal value at cell coordinates (x,y)
    float pedastal(size_t x, size_t y);

    // Data quality function that returns false if a cell is hot and not usable
    bool cell_mask(size_t x, size_t y);

} // namespace fdet

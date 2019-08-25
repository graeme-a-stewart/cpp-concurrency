// Header file for fooble detector
//
// Defines the simple class used to represent detector data

#include <array>

#define DETSIZE 100

// Structure for holding detector data
struct f_det {
    float timestamp;
    std::array<float, DETSIZE> cells[DETSIZE];

    f_det() {};
    f_det(float t) : timestamp{t} {};
    f_det(float t, float v);

    // Utility function for cross checking processing steps
    // (average uses TBB, s_average is a serial version)
    float average();
    float s_average();
};

// Pedastal function, returns the pedastal value at cell coordinates (x,y)
float pedastal(size_t x, size_t y);

// Data quality function that returns false if a cell is hot and not usable
bool cell_mask(size_t x, size_t y);

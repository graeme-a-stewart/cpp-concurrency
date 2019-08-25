#include <tbb/tbb.h>
#include <algorithm>

#include "fdet.hpp"

// Constructor that sets cell values
f_det::f_det(float t, float v): timestamp{t} {
    tbb:parallel_for(tbb::blocked_range2d<size_t>(0, DETSIZE, 0, DETSIZE),
    [&](tbb::blocked_range2d<size_t>& r) {
        for (size_t i=r.rows().begin(); i!=r.rows().end(); ++i) {
            for (size_t j=r.cols().begin(); j!=r.cols().end(); ++j) {
                cells[i][j] = v;
            }
        }
    });
}

// Average value
float f_det::average() {
    float total{0.0f};
    for (size_t x=0; x<DETSIZE; ++x) {
        for (size_t y=0; y<DETSIZE; ++y) {
            total+=cells[x][y];
        }
    }
    return total / (DETSIZE * DETSIZE);
}

// Pedastal values
// Pedastal is ~bowl shaped, low in the centre cells, but 
// higher at the edges
float pedastal(size_t x, size_t y) {
    float pedastal_value{5.0f};
    size_t edge_distance = std::min(std::min(x, DETSIZE-x), std::min(y, DETSIZE-y));
    if (edge_distance < 5) {
        pedastal_value += (5 - edge_distance) * (5 - edge_distance);
    }
    return pedastal_value;
}


// Returns true for cells that are good and false for cells
// which should not be used
static const std::vector<std::pair<size_t, size_t>> hot_cells =
    {{20, 40}, {20, 41}, {21, 41}, {34, 90}, {77, 67}};

bool cell_mask(size_t x, size_t y) {
    for (auto c: hot_cells) {
        if (c.first==x && c.second==y) {
            return false;
        }
    }
    return true;
}

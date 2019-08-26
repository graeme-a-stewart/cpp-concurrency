#include <tbb/tbb.h>
#include <algorithm>
#include <fstream>

#include "fdet.hpp"

// Constructor that sets cell values
f_det::f_det(float t, float v): timestamp{t} {
    tbb:parallel_for(tbb::blocked_range2d<size_t>(0, DETSIZE, 0, DETSIZE),
    [&](tbb::blocked_range2d<size_t>& r) {
        for (size_t x=r.rows().begin(); x!=r.rows().end(); ++x) {
            for (size_t y=r.cols().begin(); y!=r.cols().end(); ++y) {
                cells[x][y] = v;
            }
        }
    });
}

// Average value
// Define a helper class to take advantage of parallel_reduce
class fdet_sum {
private:
    float my_sum;
    f_det *my_det;

public:
    fdet_sum(f_det *fdet_p): my_sum(0.0f), my_det(fdet_p) {}
    fdet_sum(const fdet_sum &x, tbb::split): my_sum(0.0f), my_det(x.my_det) {}

    void operator()(const tbb::blocked_range2d<size_t>& r){
        for (size_t x=r.rows().begin(); x!=r.rows().end(); ++x) {
            for (size_t y=r.cols().begin(); y!=r.cols().end(); ++y) {
                my_sum += my_det->cells[x][y];
            }
        }
    }

    void join (fdet_sum& o){
        my_sum += o.my_sum;
    }

    const float sum() {
        return my_sum;
    }
};

float f_det::average() {
    fdet_sum tmp_sum(this);
    tbb::parallel_reduce(tbb::blocked_range2d<size_t>(0, DETSIZE, 0, DETSIZE), tmp_sum);
    return tmp_sum.sum() / (DETSIZE * DETSIZE);
}

float f_det::s_average() {
    float total{0.0f};
    for (size_t x=0; x<DETSIZE; ++x) {
        for (size_t y=0; y<DETSIZE; ++y) {
            total+=cells[x][y];
        }
    }
    return total / (DETSIZE * DETSIZE);
}

int f_det::read(std::ifstream &input_fp) {
    input_fp.read(reinterpret_cast<char*>(&timestamp), sizeof(float));
    input_fp.read(reinterpret_cast<char*>(&cells), sizeof(float)*DETSIZE*DETSIZE);
    if (!input_fp.good()) return 1;
    return 0;
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

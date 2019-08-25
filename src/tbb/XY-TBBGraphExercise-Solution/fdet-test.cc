#include <iostream>

#include "fdet.hpp"

int main() {
    f_det a{2}, b{2, 5};

    std::cout << a.timestamp << std::endl;
    std::cout << b.timestamp << " " << b.cells[5][5] << std::endl;

    return 0;
}

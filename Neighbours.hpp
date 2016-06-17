#pragma once

#include <cstdio>
#include <stdexcept>
#include <boost/dynamic_bitset.hpp>

typedef uint8_t bit_t;


class Neighbours {
public:

    Neighbours(boost::dynamic_bitset<>& screen, uint16_t dim):
        screen(screen),
        dim(dim) {}

    void reset() { row = 1; col = 1; };

    size_t size() const { return size_; };

    void push_back(bool bit) {
        col++;
        if (col == dim-1) {
            col = 1;
            row++;
        }
        size_++;
    };

    bool operator[](size_t pos) const {
        size_t idx = size_-1-pos;
//printf("pos=%lu idx=%lu row=%d col=%d\n", pos, idx, row, col);
        switch(size_-1-pos) {
            case 0:
                return at(row, col-1);
            case 1:
                return at(row-1, col);
            case 2:
                return at(row-1, col-1);
            case 3:
                return at(row-1, col+1);
            default:
                throw std::logic_error("only 4 parents");
        }
    };

private:

    bit_t at(size_t r, size_t c) const {
        size_t i = r*dim+c;
        return screen[i];
    }

    boost::dynamic_bitset<>& screen;

    uint16_t row{1};
    uint16_t col{1};
    uint16_t dim;
    size_t size_{4};
};

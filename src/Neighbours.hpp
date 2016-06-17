#pragma once

#include <cstdio>
#include <stdexcept>
#include <boost/dynamic_bitset.hpp>


class Neighbours {
public:

    Neighbours(uint16_t dim): dim(dim) {}

    void reset(uint8_t* screen) {
        this->screen = screen;
        size_=4; 
        mask = 1; 
        col = 1;
        row = 1;
    }

    size_t size() const { return size_; };

    void push_back(uint8_t) {
        size_++;
        mask <<= 1;
        if (mask==0){
            mask = 1;
            col++;
            if (col == dim-1) {
                row++;
                col = 1;
            }
        }
    };

    bool operator[](size_t pos) const {

        uint8_t pixel;

//printf("pos=%lu row=%d col=%d mask=%d\n", pos, row, col, mask);

        switch(size_ - 1 - pos) {
            case 0:
                pixel = at(row, col-1);
                break;
            case 1:
                pixel = at(row-1, col);
                break;
            case 2:
                pixel = at(row-1, col-1);
                break;
            case 3:
                pixel = at(row-1, col+1);
                break;
            default:
                throw std::logic_error("only 4 parents");
        }
        return (pixel & mask) > 0;
    };

private:

    uint8_t at(size_t r, size_t c) const {
        size_t i = r*dim+c;
        return screen[i];
    }


    uint8_t mask;
    uint16_t col;
    uint16_t row;
    uint8_t* screen;

    const uint16_t dim;
    size_t size_{4};
};

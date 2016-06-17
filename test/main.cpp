#include <random>
#include <iostream>
#include <cstdio>
#include <boost/dynamic_bitset.hpp>

#include "../factor.hpp"
#include "../common.hpp"
#include "../ctw.hpp"
#include "../cts.hpp"

const uint16_t dim = 3;
const uint16_t factorNum = 8 * (dim-2) * (dim-1);

typedef Factor<ContextTree, factorNum> FactoredContextTree;
typedef Factor<SwitchingTree, factorNum> FactoredSwitchingTree;

std::random_device rd;

const char* grayscale = " .:-=+*#%@";


void printScreen(uint8_t* screen) {
    size_t i = 0;
    for (size_t r = 0; r < dim; r++) {
        for (size_t c = 0; c < dim; c++) {
            uint8_t pixel = screen[i++];
            char code;
            for (uint8_t z=0; ; z++) {
                code = grayscale[z];
                if (pixel <= (z+1)*26) break;
            }
            printf("%c", code);
        }
        printf("\n");
    }
    printf("\n");
}


void randomScreen(uint8_t* screen) {
    std::mt19937 random(rd());
    std::uniform_int_distribution<uint8_t> uniform(0, 255);
    for (size_t i=0; i < dim*dim; i++) {
        screen[i] = uniform(random);
    }    
}


void drawScreen(uint8_t* screen) {
    for (size_t i=0; i<dim*dim; i++) screen[i] = 30;

    for (size_t r = 4; r < 7; r++) {
        for (size_t c = 4; c < 7; c++) {
            screen[r*dim+c] = 255;
        }
    }
}


bit_t at(size_t r, size_t c, size_t dim, uint8_t* screen) {
    return screen[r*dim+c];
}


int main(int argc, char *argv[]) {
    uint32_t times = argc>1 ? std::stoi(argv[1]) : 1;

    uint8_t* screen = new uint8_t[dim*dim];
//    drawScreen(screen);
    randomScreen(screen);
    printScreen(screen);

//    uint8_t* screen2 = new uint8_t[dim*dim];
//    drawScreen2(screen2);
//    printScreen(screen2);

    history_t history(screen, dim);

    FactoredContextTree tree(history, 4);
//    FactoredSwitchingTree tree(history, 4);

    int reportEvery = 1 + times / 50;

    for (size_t z=0; z < times; z++) {
        uint8_t* screen1 = screen;

        history.reset();
        for (size_t r = 1; r < dim; r++) {
            for (size_t c = 1; c < dim-1; c++) {
                uint8_t pixel = screen1[r*dim+c];
                uint8_t mask = 1;
                for (size_t b = 0; b < 8; b++) {
                    bit_t bit = (mask & pixel) > 0;
                    tree.update(bit);
                    mask <<= 1;
                }
            }
        }

        double sumProb = 1.;
//printf("done\n");
        history.reset();
        for (size_t r = 1; r < dim; r++) {
            for (size_t c = 1; c < dim-1; c++) {
                uint8_t pixel = screen1[r*dim+c];
                uint8_t mask = 1;
                for (size_t b = 0; b < 8; b++) {
                    bit_t bit = (mask & pixel) > 0;
                    double prob = tree.prob(bit);
                    mask <<= 1;
                    history.push_back(0);
                    sumProb *= prob;
                }
            }
        }
        if (z % reportEvery==0)
        printf("P=%.20f\n", sumProb);
    }

}


#include <random>
#include <iostream>
#include <cstdio>
#include <boost/dynamic_bitset.hpp>

#include "../factor.hpp"
#include "../common.hpp"
#include "../ctw.hpp"
#include "../cts.hpp"

const uint16_t dim = 4;
const uint16_t SIZE = dim*dim;
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


void randomScreen(std::mt19937& random, uint8_t* screen) {
    std::uniform_int_distribution<uint8_t> uniform255(0, 255);
    for (size_t i=0; i < SIZE; i++) {
        screen[i] = uniform255(random);
    }    
}


int main(int argc, char *argv[]) {
    uint32_t times = argc>1 ? std::stoi(argv[1]) : 1;

    std::mt19937 random(rd());
    std::uniform_int_distribution<uint16_t> uniformN(0, SIZE-1);

    uint8_t* screen = new uint8_t[SIZE];
    randomScreen(random, screen);
    printScreen(screen);

    history_t history(dim);

//    FactoredContextTree tree(history, 4);
    FactoredSwitchingTree tree(history, 4);

    int reportEvery = 1 + times / 50;

    for (size_t z=0; z < times; z++) {

        if (z % 10 == 0) {
            screen[uniformN(random)]++;
        }

        history.reset(screen);
        for (size_t r = 1; r < dim; r++) {
            for (size_t c = 1; c < dim-1; c++) {
                uint8_t pixel = screen[r*dim+c];
                uint8_t mask = 1;
                for (size_t b = 0; b < 8; b++) {
                    bit_t bit = (mask & pixel) > 0;
                    tree.update(bit);
                    mask <<= 1;
                }
            }
        }

        double sumProb = 1.;

        history.reset(screen);
        for (size_t r = 1; r < dim; r++) {
            for (size_t c = 1; c < dim-1; c++) {
                uint8_t pixel = screen[r*dim+c];
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
        printf("n=%5lu P=%.20f\n", z, sumProb);
    }

}


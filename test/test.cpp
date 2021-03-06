#include <random>
#include <iostream>
#include <cstdio>
#include <boost/dynamic_bitset.hpp>
#include <cassert>

#include "../src/factor.hpp"
#include "../src/common.hpp"
#include "../src/ctw.hpp"
#include "../src/cts.hpp"

const uint16_t dim = 5;
const uint16_t SIZE = dim*dim;

typedef Factor<ContextTree> FactoredContextTree;
typedef Factor<SwitchingTree> FactoredSwitchingTree;

std::random_device rd;

const char* grayscale = "_.:-=+*#%@";

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


void randomScreen(std::mt19937& random, uint8_t* screen, uint8_t range) {
    std::uniform_int_distribution<uint8_t> uniform255(0, range);
    for (size_t i=0; i < SIZE; i++) {
        screen[i] = uniform255(random);
    }    
}


double test(uint32_t times) {
    std::mt19937 random(rd());
    std::uniform_int_distribution<uint16_t> uniformN(0, SIZE-1);

    uint8_t* screen = new uint8_t[SIZE];
    randomScreen(random, screen, 0);
    printScreen(screen);

    history_t history(dim);

//    FactoredContextTree tree(dim, history, 4);
    FactoredSwitchingTree tree(dim, history, 4);

    int reportEvery = 1 + times / 50;

    double probability = 1.;

    for (size_t z=0; z < times; z++) {
        if (z % 10 == 0) {
//            screen[uniformN(random)]++;
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

        double sumProb = 0.;
        history.reset(screen);
        probability = 1.;
        for (size_t r = 1; r < dim; r++) {
            for (size_t c = 1; c < dim-1; c++) {
                uint8_t pixel = screen[r*dim+c];
                uint8_t mask = 1;
                for (size_t b = 0; b < 8; b++) {
                    bit_t bit = (mask & pixel) > 0;
                    double prob = tree.prob(bit);
                    mask <<= 1;
                    history.push_back(0);
                    sumProb += prob;
                    probability *= prob;
                }
            }
        }

        if (z % reportEvery==0) {
           printf("n=%5lu P_=%.20f P=%.20f\n", z, sumProb / (8*SIZE), probability);
        }
    }
    delete[] screen;
    return probability;
}

int main(int argc, char *argv[]) {
    uint32_t times = argc>1 ? std::stoi(argv[1]) : 1;
    assert(test(times)>0.85);
}


#include <random>
#include <iostream>
#include <cstdio>
#include <boost/dynamic_bitset.hpp>

#include "factor.hpp"
#include "common.hpp"
#include "ctw.hpp"

const uint16_t dim = 10;
const uint16_t factorNum = (dim-2) * (dim-1);
const uint16_t size = dim*dim;

typedef Factor<ContextTree, factorNum> FactoredContextTree;


std::random_device rd;

void printScreen(boost::dynamic_bitset<>& screen) {
    size_t i = 0;
    for (size_t r = 0; r < dim; r++) {
        for (size_t c = 0; c < dim; c++) {
            printf("%d", (uint8_t) screen[i++]);
        }
        printf("\n");
    }
    printf("\n");
}

void randomScreen(boost::dynamic_bitset<>& screen) {
    std::mt19937 random(rd());
    std::uniform_int_distribution<uint8_t> uniform(0, 1);
    for (size_t i=0; i < dim*dim; i++) {
        screen[i] = uniform(random);
    }    
}

void drawScreen(boost::dynamic_bitset<>& screen) {
    for (size_t r = 4; r < 7; r++) {
        for (size_t c = 4; c < 7; c++) {
            size_t i = r*dim + c;
            screen[i] = 1;
        }
    }
}

void drawScreen2(boost::dynamic_bitset<>& screen) {
    for (size_t r = 4; r < 8; r++) {
        for (size_t c = 4; c < 8; c++) {
            size_t i = r*dim + c;
            screen[i] = 1;
        }
    }
}

bit_t at(size_t r, size_t c, size_t dim, boost::dynamic_bitset<>& screen) {
    size_t i = r*dim+c;
    return screen[i];
}

int main(int argc, char *argv[]) {
    uint32_t times = argc>1 ? std::stoi(argv[1]) : 1;

    boost::dynamic_bitset<> screen(dim*dim);
    boost::dynamic_bitset<> screen2(dim*dim);
    drawScreen(screen);
    drawScreen2(screen2);
    printScreen(screen);
    printScreen(screen2);

    history_t history(screen, dim);

    FactoredContextTree tree(history, 4);

    int reportEvery = 1 + times / 50;

    for (size_t z=0; z < times; z++) {
        boost::dynamic_bitset<>& screen1 = z % 2 == 0 ? screen : screen2;
        history.reset();
        for (size_t r = 1; r < dim; r++) {
            for (size_t c = 1; c < dim-1; c++) {
                tree.update(screen1[r*dim+c]);
            }
        }

        double sumProb = 1.;

        history.reset();
        for (size_t r = 1; r < dim; r++) {
            for (size_t c = 1; c < dim-1; c++) {
                double prob = tree.prob(screen1[r*dim+c]);
                history.push_back(0);
                sumProb *= prob;
            }
        }
        if (z % reportEvery==0)
        printf("P=%.20f\n", sumProb);
    }

}


#include <random>
#include <iostream>
#include <cstdio>
#include <boost/dynamic_bitset.hpp>

#include "factor.hpp"
#include "common.hpp"
#include "ctw.hpp"

const uint16_t dim = 10;
const uint16_t factorNum = dim * dim;

typedef Factor<ContextTree, factorNum> FactoredContextTree;

history_t history;

std::random_device rd;    

void printScreen(size_t dim, boost::dynamic_bitset<>& screen) {
    size_t i = 0;
    for (size_t r = 0; r < dim; r++) {
        for (size_t c = 0; c < dim; c++) {
            printf("%d", (uint8_t) screen[i++]);
        }
        printf("\n");
    }
}

void randomScreen(size_t dim, boost::dynamic_bitset<>& screen) {
    std::mt19937 random(rd());
    std::uniform_int_distribution<uint8_t> uniform(0, 1);
    for (size_t i=0; i < factorNum; i++) {
        screen[i] = uniform(random);
    }    
}

void drawScreen(size_t dim, boost::dynamic_bitset<>& screen) {
    for (size_t r = 4; r < 7; r++) {
        for (size_t c = 4; c < 7; c++) {
            size_t i = r*dim + c;
            screen[i] = 1;
        }
    }
}


int main(int argc, char *argv[]) {
    int times = std::stoi(argv[1]);

    for (size_t i=0; i < 4; ++i) history.push_back(0);

    boost::dynamic_bitset<> screen(factorNum);
    drawScreen(dim, screen);
    printScreen(dim, screen);

    FactoredContextTree c(history, 4);

    for (size_t z=0; z < times; z++) {

        for (size_t i=0; i<factorNum; i++) {
            c.update(screen[i]);
        }

        double sumProb = 1.;

        for (size_t i=0; i<factorNum; i++) {
            double prob = c.prob(screen[i]);

            sumProb *= prob;
        }
        printf("P=%f\n", sumProb);
    }
}

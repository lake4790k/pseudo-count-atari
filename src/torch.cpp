#include <cstdio>

#include "factor.hpp"
#include "common.hpp"
#include "ctw.hpp"
#include "cts.hpp"

const uint16_t dim = 10;
const uint16_t factorNum = 8 * (dim-2) * (dim-1);

//typedef Factor<ContextTree, factorNum> FactoredContextTree;
typedef Factor<SwitchingTree, factorNum> FactoredSwitchingTree;

history_t history(dim);

extern "C" {

    void* init() {
        return new FactoredSwitchingTree(history, 4);
    }

    double probablity(void* tree_, uint8_t* screen) {
        FactoredSwitchingTree* tree = static_cast<FactoredSwitchingTree*>(tree_);
        double probability = 1.;
        history.reset(screen);
        for (size_t r = 1; r < dim; r++) {
            for (size_t c = 1; c < dim-1; c++) {
                uint8_t pixel = screen[r*dim+c];
                uint8_t mask = 1;
                for (size_t b = 0; b < 8; b++) {
                    bit_t bit = (mask & pixel) > 0;
                    double prob = tree->prob(bit);
                    mask <<= 1;
                    history.push_back(0);
                    probability *= prob;
                }
            }
        }

        history.reset(screen);
        for (size_t r = 1; r < dim; r++) {
            for (size_t c = 1; c < dim-1; c++) {
                uint8_t pixel = screen[r*dim+c];
                uint8_t mask = 1;
                for (size_t b = 0; b < 8; b++) {
                    bit_t bit = (mask & pixel) > 0;
                    tree->update(bit);
                    mask <<= 1;
                }
            }
        }

        return probability;
    }

    void finish(void* tree_) {
        FactoredSwitchingTree* tree = static_cast<FactoredSwitchingTree*>(tree_);
        delete tree;
    }
}

#include <cstdio>

#include "factor.hpp"
#include "common.hpp"
#include "ctw.hpp"
#include "cts.hpp"

//typedef Factor<ContextTree> FactoredContextTree;
typedef Factor<SwitchingTree> FactoredSwitchingTree;


extern "C" {

    void* init(uint16_t dim) {
        history_t* history = new history_t(dim);
        return new FactoredSwitchingTree(dim, *history, 4);
    }

    double probability(void* tree_, void* screen_) {
        FactoredSwitchingTree* tree = static_cast<FactoredSwitchingTree*>(tree_);
        uint16_t dim = tree->getDim();
        history_t& history = tree->getHistory();
        uint8_t* screen = static_cast<uint8_t*>(screen_);
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

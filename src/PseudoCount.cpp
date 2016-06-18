#include <cstdio>

#include "factor.hpp"
#include "common.hpp"
#include "ctw.hpp"
#include "cts.hpp"

//typedef Factor<ContextTree> FactoredContextTree;
typedef Factor<SwitchingTree> FactoredSwitchingTree;

class PseudoCount {
public:
    PseudoCount(uint16_t dim):
        dim(dim),
        history(new history_t(dim)),
        tree(new FactoredSwitchingTree(dim, *history, 4)) {}

    ~PseudoCount() {
        delete tree;
        delete history;
    }

    double pseudoCount(uint8_t* screen) {
        double probability = computeProbability(screen);
        record(screen);
        double probability_ = computeProbability(screen);

        double count = probability / (probability_ - probability);

        return count;
    }

private:

    double computeProbability(uint8_t* screen) {
        double probability = 1.;

        history->reset(screen);

        for (size_t r = 1; r < dim; r++) {
            for (size_t c = 1; c < dim-1; c++) {
                uint8_t pixel = screen[r*dim+c];
                uint8_t mask = 1;
                for (size_t b = 0; b < 8; b++) {
                    bit_t bit = (mask & pixel) > 0;
                    double prob = tree->prob(bit);
                    mask <<= 1;
                    history->push_back(0);
                    probability *= prob;
                }
            }
        }
        return probability;
    }

    void record(uint8_t* screen) {
        history->reset(screen);

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
    }

    const uint16_t dim;
    history_t* history;
    FactoredSwitchingTree* tree;
};


extern "C" {

    void* init(uint16_t dim) {
        return new PseudoCount(dim);
    }

    double pseudoCount(PseudoCount* count, uint8_t* screen) {
        return count->pseudoCount(screen);
    }

    void finish(PseudoCount* count) {
        delete count;
    }
}

#include <cstdio>

#include "factor.hpp"
#include "common.hpp"
#include "ctw.hpp"
#include "cts.hpp"

const uint16_t dim = 10;
const uint16_t factorNum = 8 * (dim-2) * (dim-1);

//typedef Factor<ContextTree, factorNum> FactoredContextTree;
typedef Factor<SwitchingTree, factorNum> FactoredSwitchingTree;

//history_t history(screen, dim);

FactoredSwitchingTree* tree;

extern "C" {

    void init() {
        if (tree) delete tree;
      //  tree = new FactoredSwitchingTree(history, 4);
    }

    double probablity(uint8_t* screen) {
        double probability = 1.;
  //      history.reset();
        for (size_t r = 1; r < dim; r++) {
            for (size_t c = 1; c < dim-1; c++) {
                double prob = tree->prob(screen[r*dim+c]);
        //        history.push_back(0);
                probability *= prob;
            }
        }

        for (size_t r = 1; r < dim; r++) {
            for (size_t c = 1; c < dim-1; c++) {
                tree->update(screen[r*dim+c]);
            }
        }

        return probability;
    }
}

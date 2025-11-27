#include <stdio.h>
#include <stdlib.h>

#include "tree/tree_lib.h"
#include "differentiator.h"
#include "optimazer.h"

int main(void) {
    MAKE_DFR(dfr_tree, calc_trees_array, 5);

    TreeReadDB("inp.txt", &dfr_tree);

    OptimizeTree(&dfr_tree);

    DiffSolveEquation(&dfr_tree);

    DiffDifferentiateEquation(&dfr_tree, &calc_trees_array, 3);

    TreeDtor(&dfr_tree);

    return 0;
}

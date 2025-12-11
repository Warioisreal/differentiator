#include <stdio.h>
#include <stdlib.h>

#include "tree/tree_lib.h"
#include "differentiator.h"
#include "optimazer.h"
#include "gnuplot.h"

int main(void) {
    MAKE_DFR(dfr_tree, calc_trees_array, dfr_tex, 8);

    TreeReadDB("input.txt", &dfr_tree, &dfr_tex);

    OptimizeTree(&dfr_tree, &dfr_tex);

    DiffUserFindDerivative(&dfr_tree, &calc_trees_array, &dfr_tex);

    DiffUserCreateTaylorSeries(&dfr_tree, &calc_trees_array, &dfr_tex);

    MakeFuncGraphs(&dfr_tree, &calc_trees_array, &dfr_tex);

    DiffDtor(&dfr_tree, &calc_trees_array, &dfr_tex);

    return 0;
}

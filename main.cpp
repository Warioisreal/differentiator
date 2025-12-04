#include <stdio.h>
#include <stdlib.h>

#include "tree/tree_lib.h"
#include "differentiator.h"
#include "optimazer.h"
#include "gnuplot.h"

int main(void) {
    MAKE_DFR(dfr_tree, calc_trees_array, 8);

    TreeReadDB("input.txt", &dfr_tree);

    OptimizeTree(&dfr_tree);

    MakeFuncGraphs(&dfr_tree, &calc_trees_array);



//     char* latex_formula = TreeToLatex(calc_trees_array.array[0]->root);
//
//     FILE* latex_file = fopen("formula.tex", "w");
//     StartLatex(latex_file);
//     fprintf(latex_file, "%s\n", latex_formula);
//     FinishLatex(latex_file);
//     free(latex_formula);


    DiffDtor(&dfr_tree, &calc_trees_array);

    return 0;
}

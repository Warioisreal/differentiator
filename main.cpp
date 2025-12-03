#include <stdio.h>
#include <stdlib.h>

#include "tree/tree_lib.h"
#include "differentiator.h"
#include "optimazer.h"

int main(void) {
    MAKE_DFR(dfr_tree, calc_trees_array, 8);

    TreeReadDB("input.txt", &dfr_tree);

    CreateTaylorSeries(&dfr_tree, &calc_trees_array, 1, 3);

    TreePrint(calc_trees_array.array[3], "abc");

//     OptimizeTree(&dfr_tree);
//
//     DiffSolveEquation(&dfr_tree);
//
    // DiffUserFindDerivative(&dfr_tree, &calc_trees_array);

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

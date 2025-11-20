#include <stdio.h>
#include <stdlib.h>

#include "tree/tree_lib.h"
#include "differentiator.h"

int main(void) {
    CalculateTables();

    MAKE_TREE(diff);

    TreeReadDB("input.txt", &diff);

    TreeDtor(&diff);
    return 0;
}

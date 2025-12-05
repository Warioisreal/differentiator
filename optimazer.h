#ifndef _OPTIMIZER_H_
#define _OPTIMIZER_H_


#include "differentiator.h"


void OptimizeTree(Tree_type* tree, LATEX* latex);

void OptimizeConstantElements(Tree_type* tree, Node_t** node, bool* is_update);
void DeleteNeutralElements(Tree_type* tree, Node_t** node, bool* is_update);

#endif //_OPTIMIZER_H_

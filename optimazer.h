#ifndef _OPTIMIZER_H_
#define _OPTIMIZER_H_


#include "differentiator.h"


dfr_return_t OptimizeTree(Tree_type* tree, LATEX* latex);

dfr_return_t OptimizeConstantElements(Tree_type* tree, Node_t** node, bool* is_update);
dfr_return_t DeleteNeutralElements(Tree_type* tree, Node_t** node, bool* is_update);

#endif //_OPTIMIZER_H_

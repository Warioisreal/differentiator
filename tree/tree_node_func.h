#ifndef _TREE_NODE_FUNC_H_
#define _TREE_NODE_FUNC_H_


#include "tree.h"


Node_t* MakeTreeElement(node_type type, union ValueData value);
size_t CalculateNodeHash(Node_t* node);

void SubTreeFillGrey(Node_t* node);

void MakeGreenElem(Node_t* node);
void MakeYellowElem(Node_t* node);
void MakeRedElem(Node_t* node);
void MakeGreyElem(Node_t* node);

#endif //_TREE_NODE_FUNC_H_

#ifndef _TREE_DB_FUNC_H_
#define _TREE_DB_FUNC_H_

#include "tree_func.h"
#include "checkers.h"
#include "tree_node_func.h"


tree_return_t TreeMakeDB(const char* filename, Tree_type* tree);
tree_return_t TreeReadDB(const char* filename, Tree_type* tree);

#endif //_TREE_DB_FUNC_H_

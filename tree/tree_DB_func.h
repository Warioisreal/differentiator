#ifndef _TREE_DB_FUNC_H_
#define _TREE_DB_FUNC_H_


#include "tree_func.h"


tree_return_t TreeMakeDB(const char* filename, Tree_type* tree, LATEX* latex);
tree_return_t TreeReadDB(const char* filename, Tree_type* tree, LATEX* latex);

#endif //_TREE_DB_FUNC_H_

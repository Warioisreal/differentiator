#ifndef _CHECKERS_H_
#define _CHECKERS_H_

#include <stdlib.h>

#include "../differentiator.h"


bool IsDouble(const char* src);
bool IsVariable(size_t hash);
bool IsOperation(size_t hash);

#endif //_CHECKERS_H_

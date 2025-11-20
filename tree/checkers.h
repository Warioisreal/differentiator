#ifndef _CHECKERS_H_
#define _CHECKERS_H_

#include <ctype.h>

#include "../differentiator.h"


bool IsDouble(const char* src);
bool IsVariable(size_t hash);
operation_type GetTypeOperation(size_t hash);

#endif //_CHECKERS_H_

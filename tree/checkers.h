#ifndef _CHECKERS_H_
#define _CHECKERS_H_

#include <ctype.h>

#include "../differentiator.h"


const double EPSILON = 10e-7;

bool IsDouble(const char* src);
bool IsVariable(size_t hash);
operation_type GetTypeOperation(size_t hash);

int CompareDouble(double a, double b);

#endif //_CHECKERS_H_

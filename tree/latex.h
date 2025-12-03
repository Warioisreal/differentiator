#ifndef _LATEX_H_
#define _LATEX_H_

#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

static const size_t LATEX_FILE_NAME_SIZE = 100;
static const size_t LATEX_EXPRESSION_SIZE = 1000;

void StartLatex(FILE* file_latex);
void FinishLatex(FILE* file_latex);
char* TreeToLatex(Node_t* node);

#endif // _LATEX_H_

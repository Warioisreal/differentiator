#ifndef _LATEX_H_
#define _LATEX_H_

#include <stdio.h>
#include <stdlib.h>

static const size_t LATEX_COMMAND_SIZE = 100;

static const size_t LATEX_FILE_NAME_SIZE = 100;
static const size_t LATEX_EXPRESSION_SIZE = 5000;

struct Node_t;

struct LATEX {
    FILE* file_latex = nullptr;
    char name[LATEX_FILE_NAME_SIZE] = "";
};

#define MAKE_LATEX(latex_name) \
    LATEX latex_name = {}; \
    snprintf(latex_name.name, LATEX_FILE_NAME_SIZE, "%s", #latex_name); \
    TechInit(&latex_name);

void TechInit(LATEX* latex);

void StartLatex(LATEX* latex);
void FinishLatex(LATEX* latex);

void TechBeginSection(LATEX* latex,    const char* title);
void TechBeginSubsubsection(LATEX* latex, const char* title);
void TechBeginSubsection(LATEX* latex, const char* title);
void TechAppendText(LATEX* latex, const char* text);
void TechAppendCommand(LATEX* latex, const char* command);

void TechBeginEquationBlock(LATEX* latex, const char* title);
void TechEndEquationBlock(LATEX* latex);
void TechAppendFormula(LATEX* latex, struct Node_t* expr);

void LatexToPDF(LATEX* latex);

void TechAddMem(LATEX* latex);

void TechAddOptMem(LATEX* latex);

#endif // _LATEX_H_

#ifndef _DIFFERENTIATOR_H_
#define _DIFFERENTIATOR_H_

#include <stdio.h>
#include <stdlib.h>


struct Tree_type;

const size_t VAR_TABLE_SIZE = 2;
const size_t OPR_TABLE_SIZE = 23;

const size_t VARIABLE_NAME_SIZE  = 10;
const size_t OPERATION_NAME_SIZE = 10;

typedef enum class NodeTypes : char {
    DEFAULT = 0,

    OPERATION = 1,
    VARIABLE  = 2,
    NUMBER    = 3
} node_type;

typedef enum class Operations : char {
    DEFAULT = 0,

    ADD = 1,
    SUB = 2,
    MUL = 3,
    DIV = 4,

    SIN  = 5,
    COS  = 6,
    TAN  = 7,
    CTAN = 8,

    ARCSIN  = 9,
    ARCCOS  = 10,
    ARCTAN  = 11,
    ARCCTAN = 12,

    SH  = 13,
    CH  = 14,
    TH  = 15,
    CTH = 16,

    ARCSH  = 17,
    ARCCH  = 18,
    ARCTH  = 19,
    ARCCTH = 20,

    LN =  21,
    EXP = 22
} operation_type;

struct Variable {
    char name[VARIABLE_NAME_SIZE] = "";
    double value = 0;
    size_t hash = 0;
};

struct Operation {
    operation_type type = operation_type::DEFAULT;
    char name[OPERATION_NAME_SIZE] = "";
    size_t hash = 0;
};

extern struct Variable VarTable[VAR_TABLE_SIZE];
extern struct Operation OprTable[OPR_TABLE_SIZE];

size_t CalculateStringHash(const char* src);
void CalculateTables(void);

double DiffSolveEquation(Tree_type* tree);
void DiffDifferentiateEquation(Tree_type* tree);

#endif //_DIFFERENTIATOR_H_

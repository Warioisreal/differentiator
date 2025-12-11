#ifndef _DIFFERENTIATOR_H_
#define _DIFFERENTIATOR_H_

#include <stdio.h>
#include <stdlib.h>

#include "tree/latex.h"


struct Tree_type;
struct Node_t;


const size_t VAR_TABLE_SIZE    = 64;
const size_t PARAMS_TABLE_SIZE = 5;
const size_t OPR_TABLE_SIZE    = 24;

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

    LOG = 21,
    POW = 22,

    D = 23
} operation_type;

typedef enum class DfrReturn : char {
    OK = 0,
    ERROR = 1
} dfr_return_t;

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

union ParamValue {
    size_t szt;
    double dbl;
};

struct Params {
    const char* param_name = nullptr;
    union ParamValue param_data;
};

extern struct Variable  VarTable[VAR_TABLE_SIZE];
extern struct Params    ParamsTable[PARAMS_TABLE_SIZE];
extern struct Operation OprTable[OPR_TABLE_SIZE];

struct ExtraTrees {
    size_t size      = 0;
    size_t capacity  = 0;
    Tree_type** array = nullptr;
};


#ifdef LOG_TREE
#define MAKE_DFR(dfr_name, calc_arr_name, latex_name, arr_size) \
    UpdateLogsFolder(); \
    MAKE_TREE(dfr_name); \
    MAKE_LATEX(latex_name); \
    ExtraTrees calc_arr_name = {}; \
    CreateSubTreesArray(&calc_arr_name, arr_size);
#else
#define MAKE_DFR(dfr_name, calc_arr_name, latex_name, arr_size) \
    MAKE_TREE(dfr_name); \
    MAKE_LATEX(latex_name); \
    ExtraTrees calc_arr_name = {}; \
    CreateSubTreesArray(&calc_arr_name, arr_size);
#endif



size_t CalculateStringHash(const char* src);
size_t GetVarNumber(const char* var_name);
void CalculateTables(size_t drv_hash, size_t drv_cnt, double dot, double rngX, double rngY);
void PrintVarTable();

void MakeFuncGraphs(Tree_type* tree, ExtraTrees* calc_array, LATEX* latex);

double DiffSolveEquation(Tree_type* tree);
void DiffUserFindDerivative(Tree_type* tree,     ExtraTrees* calc_array, LATEX* latex);
void DiffUserCreateTaylorSeries(Tree_type* tree, ExtraTrees* calc_array, LATEX* latex);

double SolveRec(Node_t* node);

dfr_return_t CreateSubTreesArray(ExtraTrees* calc_array, size_t size);

void DiffDtor(Tree_type* eq_tree, ExtraTrees* calc_array, LATEX* latex);

#endif //_DIFFERENTIATOR_H_

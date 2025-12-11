#ifndef _DIFFERENTIATOR_H_
#define _DIFFERENTIATOR_H_

#include <stdio.h>
#include <stdlib.h>

#include "define_lib.h"

#include "tree/latex.h"


struct Tree_type;
struct Node_t;


const size_t VAR_TABLE_SIZE    = 64;
const size_t PARAMS_TABLE_SIZE = 5;
const size_t OPR_TABLE_SIZE    = 24;

const size_t VARIABLE_NAME_SIZE  = 10;
const size_t OPERATION_NAME_SIZE = 10;

const size_t MAX_DRV_NUMBER = 30;


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

//----------------------------------------------------------------------------------

typedef enum class DfrReturn : char {
    OK = 0,
    ALLOC_STA_ERROR    = 1,
    GP_FUNC_FILE_ERROR = 2,
    GP_DRV_FILE_ERROR  = 3,
    GP_TLR_FILE_ERROR  = 4,
    TREE_PARAMS_ERROR  = 5,
    TREE_DATA_ERROR    = 6,
    GP_START_ERROR     = 7,
    EXTRA_TREE_PTR_ERR = 8,
    TREE_DTOR_ERROR    = 9,
    EXT_TREE_DTOR_ERR  = 10,
    NODE_PTR_ERROR     = 11
} dfr_return_t;

//----------------------------------------------------------------------------------

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

//----------------------------------------------------------------------------------

union ParamValue {
    size_t szt;
    double dbl;
};

struct Params {
    const char* param_name = nullptr;
    union ParamValue param_data;
};

//----------------------------------------------------------------------------------

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


#define CHECK_AND_RETURN(func_ret) \
    result = func_ret; \
    if (result != dfr_return_t::OK) { return result; }

#define DFR_VERIFY_AND_RETURN(tree, node, message) BEGIN { \
    tree_return_t error = TreeVerify(tree); \
    if (error != tree_return_t::TREE_OK) { \
        SubTreeDump(tree, node, message, error); \
        return dfr_return_t::TREE_PARAMS_ERROR; \
    } else { \
        error = SubTreeVerify(node); \
        if (error != tree_return_t::TREE_OK) { \
            SubTreeDump(tree, node, message, error); \
            return dfr_return_t::TREE_DATA_ERROR; \
        } \
    } \
} END


size_t CalculateStringHash(const char* src);
size_t GetVarNumber(const char* var_name);
void CalculateTables(size_t drv_hash, size_t drv_cnt, double dot, double rngX, double rngY);
void PrintVarTable();

dfr_return_t MakeFuncGraphs(Tree_type* tree, ExtraTrees* calc_array, LATEX* latex);

double DiffSolveEquation(Tree_type* tree);
dfr_return_t DiffUserFindDerivative(Tree_type* tree,     ExtraTrees* calc_array, LATEX* latex);
dfr_return_t DiffUserCreateTaylorSeries(Tree_type* tree, ExtraTrees* calc_array, LATEX* latex);

double SolveRec(Node_t* node);

dfr_return_t CreateSubTreesArray(ExtraTrees* calc_array, size_t size);

dfr_return_t DiffDtor(Tree_type* eq_tree, ExtraTrees* calc_array, LATEX* latex);

#endif //_DIFFERENTIATOR_H_

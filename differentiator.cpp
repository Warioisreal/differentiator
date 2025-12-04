#include <string.h>

#include "define_lib.h"
#include "gnuplot.h"

#include "tree/tree_func.h"
#include "math_func.h"

#include "optimazer.h"
#include "differentiator.h"


struct Variable VarTable[VAR_TABLE_SIZE] = {
    {"x", 0, 0},
    {"y", 0, 0}
};

struct Operation OprTable[OPR_TABLE_SIZE] = {
    {operation_type::DEFAULT, "DEFAULT", 0},

    {operation_type::ADD, "+", 0},
    {operation_type::SUB, "-", 0},
    {operation_type::MUL, "*", 0},
    {operation_type::DIV, "/", 0},

    {operation_type::SIN,  "sin",  0},
    {operation_type::COS,  "cos",  0},
    {operation_type::TAN,  "tan",  0},
    {operation_type::CTAN, "ctan", 0},

    {operation_type::ARCSIN,  "arcsin",  0},
    {operation_type::ARCCOS,  "arccos",  0},
    {operation_type::ARCTAN,  "arctan",  0},
    {operation_type::ARCCTAN, "arcctan", 0},

    {operation_type::SH,  "sh",  0},
    {operation_type::CH,  "ch",  0},
    {operation_type::TH,  "th",  0},
    {operation_type::CTH, "cth", 0},

    {operation_type::ARCSH,  "arcsh",  0},
    {operation_type::ARCCH,  "arcch",  0},
    {operation_type::ARCTH,  "arcth",  0},
    {operation_type::ARCCTH, "arccth", 0},

    {operation_type::LOG, "log",  0},
    {operation_type::POW, "^", 0},

    {operation_type::D, "d", 0}
};


#define SUBTREE(tree_number) tree##tree_number

#define INIT_SUBTREE \
    SUBT = (Tree_type*)calloc(1, sizeof(Tree_type)); \
    SUBT->log = (LOG*)calloc(1, sizeof(LOG)); \
    snprintf(SUBT->log->name, LOG_FILE_NAME_SIZE, "tree%zu", calc_array->size); \
    char full_folder_name[LOG_FOLDER_NAME_SIZE] = ""; \
    GetFullFolderName(SUBT->log->name, full_folder_name); \
    UpdateFolder(full_folder_name); \
    TreeCtor(SUBT); \
    TreeDtorRec(&(SUBT->root), &(SUBT->size)); \
    if (calc_array->size == calc_array->capacity - 1) { ReallocSubTreesArray(calc_array); }


static size_t djb2(size_t hash, size_t field);
static Node_t* DiffRec(Node_t* node, size_t target_hash);
static Node_t* NewNumberNode(double number);
static Node_t* NewVariableNode(const char* var);
static Node_t* NewOperationNode(operation_type op_type, Node_t* node_l, Node_t* node_r);
static Node_t* NewNode(node_type node_t, union ValueData value, Node_t* node_l, Node_t* node_r);
static Node_t* CopyNode(Node_t* node);
static void PrintVarTable();
static size_t ReadVarNumber();
static size_t GetVarNumber(const char* var_name);
static void DiffDifferentiateEquation(Tree_type* tree, ExtraTrees* calc_array, size_t degree, size_t hash);
static void CreateTaylorSeries(Tree_type* tree, ExtraTrees* calc_array, double dot, size_t accuracy, size_t var_num);
static dfr_return_t ReallocSubTreesArray(ExtraTrees* calc_array);


void CalculateTables(void) {
    for (size_t pos = 0; pos < VAR_TABLE_SIZE; pos++) {
        double val = 0;
        printf("Введите значение переменной %s:\n", VarTable[pos].name);
        scanf("%lg", &val);
        VarTable[pos].value = val;
        VarTable[pos].hash = CalculateStringHash(VarTable[pos].name);
    }

    for (size_t pos = 0; pos < OPR_TABLE_SIZE; pos++) {
        OprTable[pos].hash = CalculateStringHash(OprTable[pos].name);
    }
}

//----------------------------------------------------------------------------------

size_t CalculateStringHash(const char* src) {
    size_t hash = 0;
    for (size_t pos = 0; src[pos] != '\0'; pos++) {
        hash = djb2(hash, (size_t)src[pos]);
    }
    return hash;
}

static size_t djb2(size_t hash, size_t field) {
    return ((hash << 5) + hash) + field;
}

//----------------------------------------------------------------------------------

dfr_return_t CreateSubTreesArray(ExtraTrees* calc_array, size_t size) {
    Tree_type** array = (Tree_type**)calloc(size, sizeof(Tree_type*));

    if (array == nullptr) { return dfr_return_t::ERROR; }

    calc_array->size     = 0;
    calc_array->capacity = size;
    calc_array->array    = array;

    return dfr_return_t::OK;
}

static dfr_return_t ReallocSubTreesArray(ExtraTrees* calc_array) {
    Tree_type** array = (Tree_type**)realloc(calc_array->array, 2 * calc_array->capacity * sizeof(Tree_type*));

    if (array == nullptr) { return dfr_return_t::ERROR; }

    calc_array->capacity *= 2;
    calc_array->array    = array;

    return dfr_return_t::OK;
}

//----------------------------------------------------------------------------------

double DiffSolveEquation(Tree_type* tree) {
    double res = SolveRec(tree->root);
    printf("%lg\n", res);
    return res;
}

double SolveRec(Node_t* node) {
    if (node->type == node_type::OPERATION) {
        switch (node->value.operation) {
            case operation_type::ADD:
                return Add(SolveRec(node->left), SolveRec(node->right));
            case operation_type::SUB:
                return Sub(SolveRec(node->left), SolveRec(node->right));
            case operation_type::MUL:
                return Mul(SolveRec(node->left), SolveRec(node->right));
            case operation_type::DIV:
                return Div(SolveRec(node->left), SolveRec(node->right));
            case operation_type::SIN:
                return Sin(SolveRec(node->left));
            case operation_type::COS:
                return Cos(SolveRec(node->left));
            case operation_type::TAN:
                return Tan(SolveRec(node->left));
            case operation_type::CTAN:
                return Ctan(SolveRec(node->left));
            case operation_type::ARCSIN:
                return Arcsin(SolveRec(node->left));
            case operation_type::ARCCOS:
                return Arccos(SolveRec(node->left));
            case operation_type::ARCTAN:
                return Arctan(SolveRec(node->left));
            case operation_type::ARCCTAN:
                return Arcctan(SolveRec(node->left));
            case operation_type::SH:
                return Sinh(SolveRec(node->left));
            case operation_type::CH:
                return Cosh(SolveRec(node->left));
            case operation_type::TH:
                return Tanh(SolveRec(node->left));
            case operation_type::CTH:
                return Ctanh(SolveRec(node->left));
            case operation_type::ARCSH:
                return Arcsinh(SolveRec(node->left));
            case operation_type::ARCCH:
                return Arccosh(SolveRec(node->left));
            case operation_type::ARCTH:
                return Arctanh(SolveRec(node->left));
            case operation_type::ARCCTH:
                return Arcctanh(SolveRec(node->left));
            case operation_type::LOG:
                return Log(SolveRec(node->left), SolveRec(node->right));
            case operation_type::POW:
                return Pow(SolveRec(node->left), SolveRec(node->right));
            case operation_type::D:
                return 0;
            case operation_type::DEFAULT:
                return 0;
            default:
                printf("INVALID OPR\n");
                return 0;
        };
    } else
    if (node->type == node_type::VARIABLE) {
        size_t var_number = GetVarNumber(node->value.variable);

        if (var_number == VAR_TABLE_SIZE) {
            printf("invalid var\n");
            return 0;
        }

        return VarTable[var_number].value;
    } else
    if (node->type == node_type::NUMBER) {
        return node->value.number;
    }
    return 0;
}

//----------------------------------------------------------------------------------

void DiffUserFindDerivative(Tree_type* tree, ExtraTrees* calc_array) {
    printf("По какой переменной дифференцировать?\n");

    PrintVarTable();

    size_t var_num = ReadVarNumber();

    if (var_num == VAR_TABLE_SIZE) {
        printf("invalid var\n");
        return;
    }

    size_t var_hash = VarTable[var_num].hash;

    printf("Какую по счёту производную найти?\n");

    size_t power = 0;

    scanf("%zu", &power);

    DiffDifferentiateEquation(tree, calc_array, power, var_hash);

    printf("DIR_%zu_TREE_NAME: %s\n",
        power,
        calc_array->array[calc_array->size - 1]->log->name);
}

void DiffUserCreateTaylorSeries(Tree_type* tree, ExtraTrees* calc_array) {
    printf("По какой переменной строить ряд?\n");

    PrintVarTable();

    size_t var_num = ReadVarNumber();

    if (var_num == VAR_TABLE_SIZE) {
        printf("invalid var\n");
        return;
    }

    printf("В окрестности какой точки?\n");
    double dot = 0;
    scanf("%lg", &dot);

    printf("С какой точностью?\n");
    size_t accuracy = 0;
    scanf("%zu", &accuracy);

    CreateTaylorSeries(tree, calc_array, dot, accuracy, var_num);
}

//=================================================================================
#define ARR calc_array->array
#define SIZE calc_array->size
#define SUBT ARR[SIZE]
//=================================================================================

#define GET_PARAMS \
    printf("Какая переменная является аргументом?\n"); \
    PrintVarTable(); \
    size_t var_num = ReadVarNumber(); \
    if (var_num == VAR_TABLE_SIZE) { \
        printf("invalid var\n"); \
        return; \
    } \
    printf("В окрестности какой точки?\n"); \
    double dot = 0; \
    scanf("%lg", &dot); \
    printf("С какой точностью?\n"); \
    size_t accuracy = 0; \
    scanf("%zu", &accuracy);

#define INIT_FILENAMES \
    const char* func_filename = "function_data.txt"; \
    const char* drvt_filename = "derivative_data.txt"; \
    const char* tayl_filename = "taylor_srs_data.txt";

#define GET_FULL_FILE_PATHS \
    char func_file_path[GP_DATA_FILENAME_LEN] = ""; \
    char drvt_file_path[GP_DATA_FILENAME_LEN] = ""; \
    char tayl_file_path[GP_DATA_FILENAME_LEN] = ""; \
    BuildGPDataFilePath(func_file_path, data_foldername, func_filename); \
    BuildGPDataFilePath(drvt_file_path, data_foldername, drvt_filename); \
    BuildGPDataFilePath(tayl_file_path, data_foldername, tayl_filename);

#define OPEN_FILES \
    FILE* gp_file_function   = OpenDataGnuplotFile(func_file_path); \
    FILE* gp_file_derivative = OpenDataGnuplotFile(drvt_file_path); \
    FILE* gp_file_taylor_srs = OpenDataGnuplotFile(tayl_file_path);

#define WRITE_DATA_TO_FILES \
    AddDotToDataGnuplotFile(gp_file_function,   argument, SolveRec(tree->root)); \
    AddDotToDataGnuplotFile(gp_file_derivative, argument, SolveRec(ARR[SIZE - accuracy - 1]->root)); \
    AddDotToDataGnuplotFile(gp_file_taylor_srs, argument, SolveRec(ARR[SIZE - 1]->root));

#define CLOSE_FILES \
    CloseDataGnuplotFile(gp_file_function); \
    CloseDataGnuplotFile(gp_file_derivative); \
    CloseDataGnuplotFile(gp_file_taylor_srs);

#define ADD_FUNCTIONS_TO_GNUPLOT \
    WriteGnuplotCMD(gp_file, "plot "); \
    AddFuncGraphGnuplot(gp_file, func_file_path, "Функция", 0xFF0000); \
    WriteGnuplotCMD(gp_file, ", "); \
    AddFuncGraphGnuplot(gp_file, drvt_file_path, "Производная", 0x00FF00); \
    WriteGnuplotCMD(gp_file, ", "); \
    AddFuncGraphGnuplot(gp_file, tayl_file_path, "ряд Тейлора", 0x0000FF); \
    WriteGnuplotCMD(gp_file, "\n");


void MakeFuncGraphs(Tree_type* tree, ExtraTrees* calc_array) {
    GET_PARAMS;

    CreateTaylorSeries(tree, calc_array, dot, accuracy, var_num);

    double default_value = VarTable[var_num].value;

    const char* data_foldername = "gp_data";
    UpdateGPDataFolder(data_foldername);

    INIT_FILENAMES;
    GET_FULL_FILE_PATHS;
    OPEN_FILES;

    for (double argument = dot - 5; argument < dot + 5; argument += 0.05) {
        VarTable[var_num].value = argument;

        WRITE_DATA_TO_FILES;
    }

    CLOSE_FILES;

    VarTable[var_num].value = default_value;


    FILE* gp_file = StartGnuplot(1.5, 5, dot);
    ADD_FUNCTIONS_TO_GNUPLOT;
    FinishGnuplot(gp_file);

    CreateGnuplotGraph("plot_script.gp");
}

#undef GET_PARAMS
#undef INIT_FILENAMES
#undef GET_FULL_FILE_PATHS
#undef OPEN_FILES
#undef WRITE_DATA_TO_FILES
#undef CLOSE_FILES
#undef ADD_FUNCTIONS_TO_GNUPLOT

//----------------------------------------------------------------------------------

static void DiffDifferentiateEquation(Tree_type* tree, ExtraTrees* calc_array, size_t power, size_t hash) {
    for (size_t curent_power = 0; curent_power < power; curent_power++) {
        INIT_SUBTREE;

        if (curent_power == 0) {
            SUBT->root = DiffRec(tree->root, hash);
        } else {
            SUBT->root = DiffRec(ARR[SIZE - 1]->root, hash);
        }

        TreeCountNodes(SUBT->root, &(SUBT->size));

        TreePrint(SUBT, "DIR");

        OptimizeTree(SUBT);

        SIZE++;
    }
}

//=================================================================================
#define dL DiffRec(node->left,  target_hash)
#define dR DiffRec(node->right, target_hash)
#define cL CopyNode(node->left)
#define cR CopyNode(node->right)
#define aNn(num) NewNumberNode(num)
#define e_  NewNumberNode(GetE())
#define pi_ NewNumberNode(GetPi())
#define ADD_(left, right) NewOperationNode(operation_type::ADD, left, right)
#define SUB_(left, right) NewOperationNode(operation_type::SUB, left, right)
#define MUL_(left, right) NewOperationNode(operation_type::MUL, left, right)
#define DIV_(left, right) NewOperationNode(operation_type::DIV, left, right)
#define SN_(left)   NewOperationNode(operation_type::SIN,     left, nullptr)
#define CS_(left)   NewOperationNode(operation_type::COS,     left, nullptr)
#define TN_(left)   NewOperationNode(operation_type::TAN,     left, nullptr)
#define CTN_(left)  NewOperationNode(operation_type::CTAN,    left, nullptr)
#define ASN_(left)  NewOperationNode(operation_type::ARCSIN,  left, nullptr)
#define ACS_(left)  NewOperationNode(operation_type::ARCCOS,  left, nullptr)
#define ATN_(left)  NewOperationNode(operation_type::ARCTAN,  left, nullptr)
#define ACTN_(left) NewOperationNode(operation_type::ARCCTAN, left, nullptr)
#define SH_(left)   NewOperationNode(operation_type::SH,      left, nullptr)
#define CH_(left)   NewOperationNode(operation_type::CH,      left, nullptr)
#define TH_(left)   NewOperationNode(operation_type::TH,      left, nullptr)
#define CTH_(left)  NewOperationNode(operation_type::CTH,     left, nullptr)
#define ASH_(left)  NewOperationNode(operation_type::ARCSH,   left, nullptr)
#define ACH_(left)  NewOperationNode(operation_type::ARCCH,   left, nullptr)
#define ATH_(left)  NewOperationNode(operation_type::ARCTH,   left, nullptr)
#define ACTH_(left) NewOperationNode(operation_type::ARCCTH,  left, nullptr)
#define LOG_(left, right) NewOperationNode(operation_type::LOG, left, right)
#define LN_(left) LOG_(left, e_)
#define POW_(left, right) NewOperationNode(operation_type::POW, left, right)
#define D_(left, right) NewOperationNode(operation_type::D, left, right)
//=================================================================================

static Node_t* DiffRec(Node_t* node, size_t target_hash) {
    switch (node->type) {
        case node_type::OPERATION:
            switch (node->value.operation) {
                case operation_type::ADD:
                    return ADD_(dL, dR);
                case operation_type::SUB:
                    return SUB_(dL, dR);
                case operation_type::MUL:
                    return ADD_(MUL_(dL, cR), MUL_(cL, dR));
                case operation_type::DIV:
                    return DIV_(SUB_(MUL_(dL, cR), MUL_(cL, dR)), POW_(cR, aNn(2)));
                case operation_type::SIN:
                    return CS_(cL);
                case operation_type::COS:
                    return MUL_(SN_(cL), aNn(-1));
                case operation_type::TAN:
                    return DIV_(dL, POW_(CS_(cL), aNn(2)));
                case operation_type::CTAN:
                    return DIV_(MUL_(dL, aNn(-1)), POW_(SN_(cL), aNn(2)));
                case operation_type::ARCSIN:
                    return DIV_(dL, POW_(SUB_(aNn(1), POW_(cL, aNn(2))), aNn(1/2)));
                case operation_type::ARCCOS:
                    return DIV_(MUL_(dL, aNn(-1)), POW_(SUB_(aNn(1), POW_(cL, aNn(2))), aNn(1/2)));
                case operation_type::ARCTAN:
                    return DIV_(dL, ADD_(aNn(1), POW_(cL, aNn(2))));
                case operation_type::ARCCTAN:
                    return DIV_(dL, ADD_(aNn(-1), POW_(cL, aNn(2))));
                case operation_type::SH:
                    return CH_(cL);
                case operation_type::CH:
                    return SH_(cL);
                case operation_type::TH:
                    return DIV_(dL, POW_(CH_(cL), aNn(2)));
                case operation_type::CTH:
                    return DIV_(MUL_(aNn(-1), dL), POW_(SH_(cL), aNn(2)));
                case operation_type::ARCSH:
                    return DIV_(dL, POW_(ADD_(POW_(cL, aNn(2)), aNn(1)), aNn(1/2)));
                case operation_type::ARCCH:
                    return DIV_(dL, POW_(SUB_(POW_(cL, aNn(2)), aNn(1)), aNn(1/2)));
                case operation_type::ARCTH:
                    return DIV_(dL, SUB_(aNn(1), POW_(cL, aNn(2))));
                case operation_type::ARCCTH:
                    return DIV_(dL, SUB_(aNn(-1), POW_(cL, aNn(2))));
                case operation_type::LOG:
                    if (node->right->type == node_type::NUMBER) {
                        return DIV_(dL, MUL_(LN_(cR), cL));
                    } else
                    if (node->left->type == node_type::NUMBER) {
                        return MUL_(aNn(-1), DIV_(MUL_(LN_(cL), DIV_(dR, cR)), POW_(LN_(cR), aNn(2))));
                    } else {
                        return DIV_(SUB_(MUL_(DIV_(dL, cL), LN_(cR)), MUL_(DIV_(dR, cR), LN_(cL))), POW_(LN_(cR), aNn(2)));
                    }
                case operation_type::POW:
                    if (node->right->type == node_type::NUMBER) {
                        return MUL_(cR, MUL_(POW_(cL, SUB_(cR, aNn(1))), dL));
                    } else
                    if (node->left->type == node_type::NUMBER) {
                        return MUL_(POW_(cL, cR), MUL_(dR, LN_(cL)));
                    } else {
                        return MUL_(POW_(cL, cR), ADD_(MUL_(dR, LN_(cL)), MUL_(cR, DIV_(dL, cL))));
                    }
                case operation_type::D:
                    return nullptr; //DiffRec()
                case operation_type::DEFAULT:
                    return nullptr;
                default:
                    printf("INVALID OPR\n");
                    return nullptr;
            };
        case node_type::VARIABLE:
            if (CalculateStringHash(node->value.variable) == target_hash) {
                return aNn(1);
            } else {
                return aNn(0);
            }
        case node_type::NUMBER:
            return aNn(0);
        case node_type::DEFAULT:
            return nullptr;
        default:
            printf("NODE %p TYPE ERROR", node);
            return nullptr;
    };
}

//----------------------------------------------------------------------------------

//======================================
#define FACT_(num) NewNumberNode()
#define aNv(var) NewVariableNode(var)
//======================================

static void CreateTaylorSeries(Tree_type* tree, ExtraTrees* calc_array, double dot, size_t accuracy, size_t var_num) {
    DiffDifferentiateEquation(tree, calc_array, accuracy, VarTable[var_num].hash);

    INIT_SUBTREE;

    double default_value = VarTable[var_num].value;
    VarTable[var_num].value = dot;

    SUBT->root = aNn(SolveRec(tree->root));
    SUBT->size = 1;
    SIZE++;

    for (size_t iter = 0; iter < accuracy; iter++) {
        Node_t* node = aNn(SolveRec(ARR[SIZE - 1 - accuracy + iter]->root));

        // прибавляем (k) производную * ([var] - dot) ^ k / k!
        ARR[SIZE - 1]->root = ADD_(ARR[SIZE - 1]->root, DIV_(MUL_(node, POW_(SUB_(aNv(VarTable[var_num].name), aNn(dot)), aNn(iter + 1))), aNn(Factorial((double)(iter + 1)))));
        ARR[SIZE - 1]->size += 1 + ARR[SIZE - 1 - accuracy + iter]->size + 8;
        TreePrint(ARR[SIZE - 1], "abc");
        OptimizeTree(ARR[SIZE - 1]);
    }

    VarTable[var_num].value = default_value;

    TreePrint(ARR[SIZE - 1], "Taylor");
}

//======================================
#undef FACT_
//======================================

//=================================================================================
#undef ARR
#undef SIZE
#undef SUBT

#undef dL
#undef dR
#undef cL
#undef cR
#undef aNn
#undef e_
#undef pi_
#undef ADD_
#undef SUB_
#undef MUL_
#undef DIV_
#undef SN_
#undef CS_
#undef TN_
#undef CTN_
#undef ASN_
#undef ACS_
#undef ATN_
#undef ACTN_
#undef SH_
#undef CH_
#undef TH_
#undef CTH_
#undef ASH_
#undef ACH_
#undef ATH_
#undef ACTH_
#undef LOG_
#undef LN_
#undef POW_
#undef D_
//=================================================================================

static Node_t* NewNumberNode(double number) {
    union ValueData value;

    value.number = number;

    return NewNode(node_type::NUMBER, value, nullptr, nullptr);
}

static Node_t* NewVariableNode(const char* var) {
    union ValueData value;

    value.variable = var;

    return NewNode(node_type::VARIABLE, value, nullptr, nullptr);
}

static Node_t* NewOperationNode(operation_type op_type, Node_t* node_l, Node_t* node_r) {
    union ValueData value;

    value.operation = op_type;

    return NewNode(node_type::OPERATION, value, node_l, node_r);
}

static Node_t* NewNode(node_type node_t, union ValueData value, Node_t* node_l, Node_t* node_r) {
    Node_t* node = MakeTreeElement(node_t, value);

    node->left  = node_l;
    node->right = node_r;

    MakeGreyElem(node);

    return node;
}

static Node_t* CopyNode(Node_t* node) {
    if (node == nullptr) { return nullptr; }

    Node_t* new_node = MakeTreeElement(node->type, node->value);

    new_node->left  = CopyNode(node->left);
    new_node->right = CopyNode(node->right);

    new_node->color    = node->color;
    new_node->bg_color = node->bg_color;

    new_node->hash = node->hash;

    return new_node;
}

//----------------------------------------------------------------------------------

void DiffDtor(Tree_type* eq_tree, ExtraTrees* calc_array) {
    TreeDtor(eq_tree);

    for (size_t pos = 0; pos < calc_array->size; pos++) {
        if (calc_array->array[pos] != nullptr) {
            TreeDtor(calc_array->array[pos]);

            free(calc_array->array[pos]->log);
            calc_array->array[pos]->log = nullptr;

            free(calc_array->array[pos]);
            calc_array->array[pos] = nullptr;
        }
    }

    free(calc_array->array);
    calc_array->array = nullptr;
}

//----------------------------------------------------------------------------------

static void PrintVarTable() {
    for (size_t pos = 0; pos < VAR_TABLE_SIZE; pos++) {
        printf("%s ", VarTable[pos].name);
    } printf("\n");
}

//----------------------------------------------------------------------------------

static size_t ReadVarNumber() {
    char var_name[VARIABLE_NAME_SIZE] = "";
    scanf("%s", var_name);

    return GetVarNumber(var_name);
}

static size_t GetVarNumber(const char* var_name) {
    size_t hash = CalculateStringHash(var_name);

    for (size_t pos = 0; pos < VAR_TABLE_SIZE; pos++) {
        if (hash == VarTable[pos].hash) {
            if (strcmp(var_name, VarTable[pos].name) == 0) {
                return pos;
            }
        }
    }

    return VAR_TABLE_SIZE;
}

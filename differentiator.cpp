#include "define_lib.h"

#include "tree/tree_func.h"
#include "math_func.h"

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

    {operation_type::LN,  "ln",  0},
    {operation_type::EXP, "exp", 0}
};


static size_t djb2(size_t hash, size_t field);
static Node_t* DiffRec(Node_t* node, size_t target_hash);
static double SolveRec(Node_t* node);
static Node_t* NewNumberNode(double number);
static Node_t* NewOperationNode(operation_type op_type, Node_t* node_l, Node_t* node_r);
static Node_t* NewNode(node_type node_t, union ValueData value, Node_t* node_l, Node_t* node_r);
static Node_t* CopyNode(Node_t* node);


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

//----------------------------------------------------------------------------------

static size_t djb2(size_t hash, size_t field) {
    return ((hash << 5) + hash) + field;
}

//----------------------------------------------------------------------------------

double DiffSolveEquation(Tree_type* tree) {
    double res = SolveRec(tree->root);
    printf("%lg\n", res);
    return res;
}

//----------------------------------------------------------------------------------

void DiffDifferentiateEquation(Tree_type* tree) {
    printf("По какой переменной дифференцировать?\n");
    for (size_t pos = 0; pos < VAR_TABLE_SIZE; pos++) {
        printf("%s ", VarTable[pos].name);
    } printf("\n");

    char var_name[VARIABLE_NAME_SIZE] = "";
    scanf("%s", var_name);
    size_t answ_hash = CalculateStringHash(var_name);

    MAKE_TREE(tree1);

    TreeDtorRec(&(tree1.root), &(tree1.size));

    tree1.root = DiffRec(tree->root, answ_hash);

    TreeCountNodes(tree1.root, &(tree1.size));

    TreeDtor(&tree1);
}

#define dL DiffRec(node->left,  target_hash)
#define dR DiffRec(node->right, target_hash)
#define cL CopyNode(node->left)
#define cR CopyNode(node->right)

#define aNn(num) NewNumberNode(num)

#define ADD_(left, right) NewOperationNode(operation_type::ADD,  left, right)
#define SUB_(left, right) NewOperationNode(operation_type::SUB,  left, right)
#define MUL_(left, right) NewOperationNode(operation_type::MUL,  left, right)
#define DIV_(left, right) NewOperationNode(operation_type::DIV,  left, right)
#define SIN_(left)        NewOperationNode(operation_type::SIN,  left, nullptr)
#define COS_(left)        NewOperationNode(operation_type::COS,  left, nullptr)
#define TAN_(left)        NewOperationNode(operation_type::TAN,  left, nullptr)
#define CTAN_(left)       NewOperationNode(operation_type::CTAN, left, nullptr)

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
                    return DIV_(SUB_(MUL_(dL, cR), MUL_(cL, dR)), MUL_(cR, cR));
                case operation_type::SIN:
                    return COS_(cL);
                case operation_type::COS:
                    return MUL_(SIN_(cL), aNn(-1));
                case operation_type::TAN:
                case operation_type::CTAN:
                case operation_type::ARCSIN:
                case operation_type::ARCCOS:
                case operation_type::ARCTAN:
                case operation_type::ARCCTAN:
                case operation_type::SH:
                case operation_type::CH:
                case operation_type::TH:
                case operation_type::CTH:
                case operation_type::ARCSH:
                case operation_type::ARCCH:
                case operation_type::ARCTH:
                case operation_type::ARCCTH:
                case operation_type::LN:
                case operation_type::EXP:
                    return nullptr;
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

#undef dL
#undef dR
#undef cL
#undef cR
#undef aNn
#undef ADD_
#undef SUB_
#undef MUL_
#undef DIV_
#undef SIN_
#undef COS_
#undef TAN_
#undef CTAN_

static Node_t* NewNumberNode(double number) {
    union ValueData value;

    value.number = number;

    return NewNode(node_type::NUMBER, value, nullptr, nullptr);
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

static double SolveRec(Node_t* node) {
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
            case operation_type::ARCCOS:
            case operation_type::ARCTAN:
            case operation_type::ARCCTAN:
            case operation_type::SH:
            case operation_type::CH:
            case operation_type::TH:
            case operation_type::CTH:
            case operation_type::ARCSH:
            case operation_type::ARCCH:
            case operation_type::ARCTH:
            case operation_type::ARCCTH:
            case operation_type::LN:
            case operation_type::EXP:
                return 0;
            case operation_type::DEFAULT:
                return 0;
            default:
                printf("INVALID OPR\n");
                return 0;
        };
    } else
    if (node->type == node_type::VARIABLE) {
        size_t hash = CalculateStringHash(node->value.variable);
        for (size_t pos = 0; pos < VAR_TABLE_SIZE; pos++) {
            if (hash == VarTable[pos].hash) {
                return VarTable[pos].value;
            }
        }
        printf("invalid var\n");
        return 0;
    } else
    if (node->type == node_type::NUMBER) {
        return node->value.number;
    }
    return 0;
}

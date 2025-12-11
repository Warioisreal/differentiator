#include <string.h>

#include "checkers.h"

#include "tree_DB_func.h"
#include "../differentiator.h"


static tree_return_t TreeGetDB(Tree_type* tree, char* buffer, Node_t* node, int* position);

static void ReadData(const char** s);
static void AddVarToTable(const char** s, size_t* size);
static char*  ReadDRVname(const char** s);
static size_t ReadDRVcnt(const char** s);

static Node_t* GetExpression(const char** s);
static Node_t* GetAddition(const char** s);
static Node_t* GetMultiplication(const char** s);
static Node_t* GetPower(const char** s);
static Node_t* GetMathFunc(const char** s, Node_t* var_node);
static Node_t* GetP(const char** s);
static Node_t* GetNumber(const char** s);
static Node_t* GetVariable(const char** s);

static void SkipSpaces(const char** s);
static double GetDouble(const char** s);

tree_return_t TreeMakeDB(const char* filename, Tree_type* tree, LATEX* latex) {
    //TREE_VERIFY_AND_RETURN(tree, tree->root, true, "ERROR BEFORE MakeDB");

    FILE* file = fopen(filename, "wb");

    if (file == nullptr) {
        TreeDump(tree, "DB FILE OPEN ERROR", tree_return_t::INVALID_DB_PTR);
        return tree_return_t::INVALID_DB_PTR;
    }

    char* buffer = (char*)calloc(MAX_BUFFER_DB_SIZE, sizeof(char));

    if (buffer == nullptr) {
        TreeDump(tree, "DB BUFFER CALLOC ERROR", tree_return_t::INVALID_BUFFER);
        return tree_return_t::INVALID_BUFFER;
    }

    int pos = 0;

    tree_return_t res = TreeGetDB(tree, buffer, tree->root, &pos);

    if (res == tree_return_t::TREE_OK) {
        fprintf(file, "%s", buffer);
        TreePrint(tree, "MAKE DB");
    }

    fclose(file);

    free(buffer);
    buffer = nullptr;

    return res;
}

static tree_return_t TreeGetDB(Tree_type* tree, char* buffer, Node_t* node, int* position) {
    //CHECK_ERROR_AND_RETURN(tree, "ERROR IN TreeGetDB", SubTreeVerify(node), tree_return_t::TREE_OK);

    if (buffer == nullptr) {
        TreeDump(tree, "TreeGetDB INVALID BUFFER", tree_return_t::INVALID_BUFFER);
        return tree_return_t::INVALID_BUFFER;
    }
    int res = 0;

    if (node->type == node_type::OPERATION) {
        res = snprintf(buffer + *position, MAX_BUFFER_DB_SIZE, "(\"%s\" ", node->value.operation);
    } else
    if (node->type == node_type::VARIABLE) {
        res = snprintf(buffer + *position, MAX_BUFFER_DB_SIZE, "(\"%s\" ", node->value.variable);
    } else
    if (node->type == node_type::NUMBER) {
        res = snprintf(buffer + *position, MAX_BUFFER_DB_SIZE, "(\"%lf\" ", node->value.number);
    }

    if (res == 0) {
        TreeDump(tree, "DB BUFFER WRITE ERROR", tree_return_t::WRITE_BUF_ERR);
        return tree_return_t::WRITE_BUF_ERR;
    }
    *position += res;

    if (node->left == nullptr) {
        res = snprintf(buffer + *position, MAX_BUFFER_DB_SIZE, "nil ");
        if (res == 0) {
            TreeDump(tree, "DB BUFFER WRITE ERROR", tree_return_t::WRITE_BUF_ERR);
            return tree_return_t::WRITE_BUF_ERR;
        }
        *position += res;
    } else {
        TreeGetDB(tree, buffer, node->left, position);
    }
    if (node->right == nullptr) {
        res = snprintf(buffer + *position, MAX_BUFFER_DB_SIZE, "nil");
        if (res == 0) {
            TreeDump(tree, "DB BUFFER WRITE ERROR", tree_return_t::WRITE_BUF_ERR);
            return tree_return_t::WRITE_BUF_ERR;
        }
        *position += res;
    } else {
        TreeGetDB(tree, buffer, node->right, position);
    }

    res = snprintf(buffer + *position, MAX_BUFFER_DB_SIZE, ") ");
    if (res == 0) {
        TreeDump(tree, "DB BUFFER WRITE ERROR", tree_return_t::WRITE_BUF_ERR);
        return tree_return_t::WRITE_BUF_ERR;
    }
    *position += res;
    return tree_return_t::TREE_OK;
}

//----------------------------------------------------------------------------------

tree_return_t TreeReadDB(const char* filename, Tree_type* tree, LATEX* latex) {
    char buffer[MAX_BUFFER_DB_SIZE] = "";

    FILE* file = fopen(filename, "rb");
    fread(buffer, sizeof(char), MAX_BUFFER_DB_SIZE, file);
    fclose(file);

    TreeDtorRec(&(tree->root), &(tree->size));

    tree->size = 0;
    const char* s = buffer;

    Node_t* node = GetExpression(&s);

    if (node == nullptr) {
        TreeDump(tree, "DB READ CREATE ROOT ERROR", tree_return_t::INVALID_ROOT);
        return tree_return_t::INVALID_ROOT;
    }
    tree->root = node;

    //TREE_VERIFY_AND_RETURN(tree, tree->root, true, "ERROR AFTER ReadDB");

    TreeCountNodes(tree->root, &(tree->size));

    TechAppendText(latex,
        "Так, умный пользователь ввел нам выражение, "
        "теперь мы будем всячески его коверкать, ломать и изливать душу");
    TechBeginEquationBlock(latex, "");
    TechAppendFormula(latex, tree->root);
    TechEndEquationBlock(latex);

    TreePrint(tree, "DUMP DB TREE");

    SubTreeFillGrey(tree->root);

    return tree_return_t::TREE_OK;
}

static Node_t* GetExpression(const char** s) {
    SkipSpaces(s);

    ReadData(s);

    if (**s != '$') { return nullptr; }
    (*s)++;

    SkipSpaces(s);
    Node_t* node = GetAddition(s);
    SkipSpaces(s);

    if (**s != '$') { return nullptr; }
    (*s)++;

    return node;
}

//==========================================
static void ReadData(const char** s) {
    size_t VarTable_size = 0;

    while (**s != '|') {
        AddVarToTable(s, &VarTable_size);
        SkipSpaces(s);
    }
    (*s)++;

    SkipSpaces(s);
    char* drv_name = ReadDRVname(s);

    size_t drv_hash = CalculateStringHash(drv_name);

    free(drv_name);
    drv_name = nullptr;

    SkipSpaces(s);
    size_t drv_cnt = ReadDRVcnt(s);

    SkipSpaces(s); (*s) += 3; SkipSpaces(s); (*s)++; SkipSpaces(s);  // skip all to value
    double dot = GetDouble(s);

    SkipSpaces(s); (*s) += 4; SkipSpaces(s); (*s)++; SkipSpaces(s);  // skip all to value
    double rngX = GetDouble(s);

    SkipSpaces(s); (*s) += 4; SkipSpaces(s); (*s)++; SkipSpaces(s); // skip all to value
    double rngY = GetDouble(s);


    CalculateTables(drv_hash, drv_cnt, dot, rngX, rngY);

}

static void AddVarToTable(const char** s, size_t* size) {
    char buffer[10] = "";
    int index = 0;

    while (isalpha(**s)) { buffer[index++] = *(*s)++; }
    buffer[index] = '\0';

    VarTable[*size] = {};
    snprintf(VarTable[*size].name, VARIABLE_NAME_SIZE, "%s", buffer);

    SkipSpaces(s); (*s)++; SkipSpaces(s);

    VarTable[*size].value = GetDouble(s);

    (*size)++;
}

static char* ReadDRVname(const char** s) {
    (*s)++; // skip "D"

    char buffer[10] = "";
    int index = 0;

    while (isalpha(**s)) { buffer[index++] = *(*s)++; }
    buffer[index] = '\0';

    SkipSpaces(s);


    return strdup(buffer);
}

static size_t ReadDRVcnt(const char** s) {
    (*s)++; // skip "="

    SkipSpaces(s);

    size_t drv_cnt = 0;

    while (isdigit(**s)) {
        drv_cnt = drv_cnt * 10 + (size_t)(**s - '0');
        (*s)++;
    }

    SkipSpaces(s);


    return drv_cnt;
}
//==========================================

static Node_t* GetAddition(const char** s) {
    SkipSpaces(s);
    Node_t* node = GetMultiplication(s);
    SkipSpaces(s);

    while (**s == '+' || **s == '-') {
        char sign = **s;
        (*s)++;
        SkipSpaces(s);
        Node_t* node2 = GetMultiplication(s);
        SkipSpaces(s);

        union ValueData value;

        if (sign == '+') {
            value.operation = operation_type::ADD;
        } else {
            value.operation = operation_type::SUB;
        }
        Node_t* new_node = MakeTreeElement(node_type::OPERATION, value);
        new_node->left  = node;
        new_node->right = node2;

        node = new_node;
    }

    return node;
}

static Node_t* GetMultiplication(const char** s) {
    SkipSpaces(s);
    Node_t* node = GetPower(s);
    SkipSpaces(s);

    while (**s == '*' || **s == '/') {
        char sign = **s;
        (*s)++;
        SkipSpaces(s);
        Node_t* node2 = GetPower(s);
        SkipSpaces(s);

        union ValueData value;

        if (sign == '*') {
            value.operation = operation_type::MUL;
        } else {
            value.operation = operation_type::DIV;
        }
        Node_t* new_node = MakeTreeElement(node_type::OPERATION, value);
        new_node->left  = node;
        new_node->right = node2;

        node = new_node;
    }

    return node;
}

static Node_t* GetPower(const char** s) {
    SkipSpaces(s);
    Node_t* node = GetP(s);
    SkipSpaces(s);

    while (**s == '^') {
        (*s)++;
        SkipSpaces(s);
        Node_t* node2 = GetP(s);
        SkipSpaces(s);

        union ValueData value;
        value.operation = operation_type::POW;

        Node_t* new_node = MakeTreeElement(node_type::OPERATION, value);
        new_node->left  = node;
        new_node->right = node2;

        node = new_node;
        SkipSpaces(s);
    }

    return node;
}

static Node_t* GetP(const char** s) {
    if (**s == '(') {
        (*s)++;
        SkipSpaces(s);
        Node_t* node = GetAddition(s);
        SkipSpaces(s);
        if (**s == ')') {
            (*s)++;
            return node;
        } else {
            return nullptr;
        }
    } else if (isdigit(**s) || (**s == '-' && isdigit(*(*s + 1))) || (**s == '.' && isdigit(*(*s + 1)))) {
        SkipSpaces(s);
        return GetNumber(s);
    } else if (isalpha(**s) || **s == '_') {
        SkipSpaces(s);
        Node_t* var_node = GetVariable(s);
        SkipSpaces(s);

        // Это функция
        if (**s == '(') {
            SkipSpaces(s);
            Node_t* func_node = GetMathFunc(s, var_node);
            SkipSpaces(s);

            return func_node;
        }

        // Это переменная
        return var_node;
    } else {
        return nullptr;
    }
}

static Node_t* GetNumber(const char** s) {
    union ValueData value;
    value.number = GetDouble(s);
    Node_t* node = MakeTreeElement(node_type::NUMBER, value);
    return node;
}

static Node_t* GetVariable(const char** s) {
    SkipSpaces(s);

    if (!(isalpha(**s) || **s == '_')) { return nullptr; }

    char var_name[10] = "";
    int index = 0;

    while (isalpha(**s) || isdigit(**s) || **s == '_') {
        var_name[index++] = *(*s)++;
    }
    var_name[index] = '\0';

    SkipSpaces(s);

    union ValueData value;
    value.variable = strdup(var_name); // возможны утечки по памяти

    if (**s != '(') {
        size_t var_num = GetVarNumber(value.variable);

        if (var_num == VAR_TABLE_SIZE) {

        }
    }
    // было бы хорошо тут записывать массив переменных

    Node_t* node = MakeTreeElement(node_type::VARIABLE, value);

    return node;
}

static Node_t* GetMathFunc(const char** s, Node_t* var_node) {
    if (var_node == nullptr || var_node->type != node_type::VARIABLE) { return nullptr; }

    // Проверяем, что после имени функции идет '('
    if (**s == '(') { (*s)++; }
    SkipSpaces(s);

    // Получаем имя функции из узла переменной
    const char* func_name = var_node->value.variable;
    size_t func_hash = CalculateStringHash(func_name);
    operation_type op = GetTypeOperation(func_name, func_hash);

    if (op == operation_type::DEFAULT) { return nullptr; }

    union ValueData value;
    value.operation = op;

    Node_t* base = nullptr;

    // Обработка для логарифма и дифференциала
    if (op == operation_type::LOG || op == operation_type::D) {
        // основание
        SkipSpaces(s);
        base = GetAddition(s);
        SkipSpaces(s);

        if (**s == ')') { (*s)++; } else { return nullptr; }

        SkipSpaces(s);

        // аргумент
        if (**s == '(') { (*s)++; }
    }

    // Обычные функции с одним аргументом
    SkipSpaces(s);
    Node_t* arg_node = GetAddition(s);
    SkipSpaces(s);

    if (**s == ')') { (*s)++; } else { return nullptr; }

    // Изменяем узел переменной на узел операции
    var_node->type  = node_type::OPERATION;
    var_node->value = value;
    var_node->left  = arg_node;
    var_node->right = base;

    return var_node;
}

//----------------------------------------------------------------------------------

static void SkipSpaces(const char** s) {
    while (isspace(**s)) {
        (*s)++;
    }
}

static double GetDouble(const char** s) {
    char number_str[100] = "";
    int index = 0;

    int sign = 1;

    if (**s == '-' && '0' <= *(1 + *s) && *(1 + *s) <= '9') {
        sign = -1;
        (*s)++;
    }

    while ('0' <= **s && **s <= '9') { number_str[index++] = *(*s)++; }

    if (**s == '.') {
        number_str[index++] = *(*s)++;

        while ('0' <= **s && **s <= '9') { number_str[index++] = *(*s)++; }
    }

    number_str[index] = '\0';

    SkipSpaces(s);


    return sign * atof(number_str);
}

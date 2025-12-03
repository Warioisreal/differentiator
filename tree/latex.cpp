#include "../differentiator.h"

#include "latex.h"


#define ADD_TO_BUFFER(text) *pos += (size_t)snprintf(buffer + *pos, LATEX_EXPRESSION_SIZE - *pos, text)


static void LatexRecursive(Node_t* node, char* buffer, size_t* pos);
static bool NeedsParentheses(Node_t* node, Node_t* parent);
static int GetPriority(operation_type op);


void StartLatex(FILE* file_latex) {
    fprintf(file_latex,
        "\\documentclass{article}\n"
        "\\usepackage{amsmath}\n"
        "\\begin{document}\n"
        "\\begin{align*}\n");
}

//----------------------------------------------------------------------------------

void FinishLatex(FILE* file_latex) {
    fprintf(file_latex,
        "\\end{align*}\n"
        "\\end{document}\n");
    fclose(file_latex);
}

//----------------------------------------------------------------------------------

char* TreeToLatex(Node_t* node) {
    char* result = (char*)calloc(LATEX_EXPRESSION_SIZE, sizeof(char));
    size_t pos = 0;

    LatexRecursive(node, result, &pos);
    result[pos] = '\0';

    return result;
}

//----------------------------------------------------------------------------------

static bool NeedsParentheses(Node_t* node, Node_t* parent) {
    if (parent == nullptr || node == nullptr) return false;

    if (node->type != node_type::OPERATION) return false;

    int parent_priority = GetPriority(parent->value.operation);
    int node_priority = GetPriority(node->value.operation);

    if (node_priority < parent_priority) return true;

    if (parent_priority == node_priority) {
        // Для неассоциативных операций
        if (parent->value.operation == operation_type::SUB ||
            parent->value.operation == operation_type::DIV) {
            return true;
        }
    }

    return false;
}


static int GetPriority(operation_type op) {
    switch (op) {
        case operation_type::POW:
            return 4;
        case operation_type::MUL:
        case operation_type::DIV:
            return 3;
        case operation_type::ADD:
        case operation_type::SUB:
            return 2;
        case operation_type::SIN:
        case operation_type::COS:
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
        case operation_type::LOG:
        case operation_type::D:
            return 1;
        case operation_type::DEFAULT:
            return 0;
        default:
            return 0;
    }
}

//----------------------------------------------------------------------------------

static void LatexRecursive(Node_t* node, char* buffer, size_t* pos) {
    if (node == nullptr) return;

    if (node->type == node_type::OPERATION) {
        bool needs_paren = false;

        switch (node->value.operation) {
            case operation_type::ADD:
                LatexRecursive(node->left, buffer, pos);
                ADD_TO_BUFFER(" + ");
                LatexRecursive(node->right, buffer, pos);
                break;

            case operation_type::SUB:
                LatexRecursive(node->left, buffer, pos);
                ADD_TO_BUFFER(" - ");
                LatexRecursive(node->right, buffer, pos);
                break;

            case operation_type::MUL:
                needs_paren = NeedsParentheses(node->left, node);
                if (needs_paren) { ADD_TO_BUFFER("("); }
                LatexRecursive(node->left, buffer, pos);
                if (needs_paren) { ADD_TO_BUFFER(")"); }

                ADD_TO_BUFFER(" \\cdot ");

                needs_paren = NeedsParentheses(node->right, node);
                if (needs_paren) { ADD_TO_BUFFER("("); }
                LatexRecursive(node->right, buffer, pos);
                if (needs_paren) { ADD_TO_BUFFER(")"); }
                break;

            case operation_type::DIV:
                ADD_TO_BUFFER("\\frac{");
                LatexRecursive(node->left, buffer, pos);
                ADD_TO_BUFFER("}{");
                LatexRecursive(node->right, buffer, pos);
                ADD_TO_BUFFER("}");
                break;

            case operation_type::POW:
                needs_paren = NeedsParentheses(node->left, node);
                if (needs_paren) { ADD_TO_BUFFER("("); }
                LatexRecursive(node->left, buffer, pos);
                if (needs_paren) { ADD_TO_BUFFER(")"); }

                ADD_TO_BUFFER("^{");
                LatexRecursive(node->right, buffer, pos);
                ADD_TO_BUFFER("}");
                break;

            case operation_type::SIN:
                ADD_TO_BUFFER("\\sin(");
                LatexRecursive(node->left, buffer, pos);
                ADD_TO_BUFFER(")");
                break;

            case operation_type::COS:
                ADD_TO_BUFFER("\\cos(");
                LatexRecursive(node->left, buffer, pos);
                ADD_TO_BUFFER(")");
                break;

            case operation_type::TAN:
                ADD_TO_BUFFER("\\tan(");
                LatexRecursive(node->left, buffer, pos);
                ADD_TO_BUFFER(")");
                break;

            case operation_type::CTAN:
                ADD_TO_BUFFER("\\cot(");
                LatexRecursive(node->left, buffer, pos);
                ADD_TO_BUFFER(")");
                break;

            case operation_type::ARCSIN:
                ADD_TO_BUFFER("\\arcsin(");
                LatexRecursive(node->left, buffer, pos);
                ADD_TO_BUFFER(")");
                break;

            case operation_type::ARCCOS:
                ADD_TO_BUFFER("\\arccos(");
                LatexRecursive(node->left, buffer, pos);
                ADD_TO_BUFFER(")");
                break;

            case operation_type::ARCTAN:
                ADD_TO_BUFFER("\\arctan(");
                LatexRecursive(node->left, buffer, pos);
                ADD_TO_BUFFER(")");
                break;

            case operation_type::ARCCTAN:
                ADD_TO_BUFFER("\\arccot(");
                LatexRecursive(node->left, buffer, pos);
                ADD_TO_BUFFER(")");
                break;

            case operation_type::SH:
                ADD_TO_BUFFER("\\sinh(");
                LatexRecursive(node->left, buffer, pos);
                ADD_TO_BUFFER(")");
                break;

            case operation_type::CH:
                ADD_TO_BUFFER("\\cosh(");
                LatexRecursive(node->left, buffer, pos);
                ADD_TO_BUFFER(")");
                break;

            case operation_type::TH:
                ADD_TO_BUFFER("\\tanh(");
                LatexRecursive(node->left, buffer, pos);
                ADD_TO_BUFFER(")");
                break;

            case operation_type::CTH:
                ADD_TO_BUFFER("\\coth(");
                LatexRecursive(node->left, buffer, pos);
                ADD_TO_BUFFER(")");
                break;

            case operation_type::ARCSH:
                ADD_TO_BUFFER("\\operatorname{arsinh}(");
                LatexRecursive(node->left, buffer, pos);
                ADD_TO_BUFFER(")");
                break;

            case operation_type::ARCCH:
                ADD_TO_BUFFER("\\operatorname{arcosh}(");
                LatexRecursive(node->left, buffer, pos);
                ADD_TO_BUFFER(")");
                break;

            case operation_type::ARCTH:
                ADD_TO_BUFFER("\\operatorname{artanh}(");
                LatexRecursive(node->left, buffer, pos);
                ADD_TO_BUFFER(")");
                break;

            case operation_type::ARCCTH:
                ADD_TO_BUFFER("\\operatorname{arcoth}(");
                LatexRecursive(node->left, buffer, pos);
                ADD_TO_BUFFER(")");
                break;

            case operation_type::LOG:
                ADD_TO_BUFFER("\\log(");
                LatexRecursive(node->left, buffer, pos);
                ADD_TO_BUFFER(")");
                break;
            case operation_type::DEFAULT:
            case operation_type::D: break;
            default:
                ADD_TO_BUFFER("?");
                break;
        }
    }
    else if (node->type == node_type::VARIABLE) {
        // Ищем переменную в таблице
        for (size_t i = 0; i < VAR_TABLE_SIZE; i++) {
            if (VarTable[i].hash == CalculateStringHash(node->value.variable)) {
                *pos += (size_t)snprintf(buffer + *pos, LATEX_EXPRESSION_SIZE - *pos, "%s", VarTable[i].name);
                return;
            }
        }
        // Если не нашли - выводим как есть
        *pos += (size_t)snprintf(buffer + *pos, LATEX_EXPRESSION_SIZE - *pos, "%s", node->value.variable);
    }
    else if (node->type == node_type::NUMBER) {
        // Форматируем числа для LaTeX
        *pos += (size_t)snprintf(buffer + *pos, LATEX_EXPRESSION_SIZE - *pos, "%.5lg", node->value.number);
    }
}

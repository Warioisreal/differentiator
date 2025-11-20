#include "differentiator.h"

struct Variable VarTable[VAR_TABLE_SIZE] = {
    {"x", 0},
    {"y", 0}
};

struct Operation OprTable[OPR_TABLE_SIZE] = {
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


void CalculateTables(void) {
    for (size_t pos = 0; pos < VAR_TABLE_SIZE; pos++) {
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

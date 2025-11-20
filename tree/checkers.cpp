#include "checkers.h"


bool IsDouble(const char* src) {
    char* end_ptr = nullptr;

    strtod(src, &end_ptr);
    if (*end_ptr == '\0') { return true; }

    return false;
}

//----------------------------------------------------------------------------------

bool IsVariable(size_t hash) {
    for (size_t pos = 0; pos < VAR_TABLE_SIZE; pos++) {
        if (VarTable[pos].hash == hash) { return true; }
    }

    return false;
}

//----------------------------------------------------------------------------------

bool IsOperation(size_t hash) {
    for (size_t pos = 0; pos < OPR_TABLE_SIZE; pos++) {
        if (OprTable[pos].hash == hash) { return true; }
    }

    return false;
}

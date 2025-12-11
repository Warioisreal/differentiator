#ifndef _TREE_H_
#define _TREE_H_


#include <stdlib.h>

#include "../differentiator.h"


const size_t MAX_ANSWER_SIZE = 1000;
const size_t MAX_BUFFER_DB_SIZE = 10000;
const size_t MAX_DUMP_MESSAGE_SIZE = 200;
const size_t HASH_SEED = 5381;


union ValueData {
    operation_type operation;
    char* variable;
    double number;
};


struct Node_t {
    node_type type = node_type::DEFAULT;

    union ValueData value;

    Node_t* left  = nullptr;
    Node_t* right = nullptr;

    size_t color    = 0;
    size_t bg_color = 0;

    size_t hash = 0;
};

typedef enum class TreeReturn : char {
    TREE_OK = 0,
    ADD_ELEMENT    = 1,
    DELETE_SUBTREE = 2,
    INVALID_ANSWER = 3,
    INCORRECT_ANSW = 4,
    INVALID_LOG = 5,
    INVALID_NODE   = 6,
    INVALID_BUFFER = 7,
    WRITE_BUF_ERR  = 8,
    INVALID_ROOT   = 9,
    INVALID_SIZE   = 10,
    INVALID_VALUE  = 11,
    INVALID_HASH   = 12,
    INVALID_CHILDS = 13,
    INVALID_DB_PTR = 14
} tree_return_t;

static const char* TreeErrorsArray[] = {
    "TREE_OK",
    "ADD_ELEMENT",
    "DELETE_SUBTREE",
    "INVALID_ANSWER",
    "INCORRECT_ANSW",
    "INVALID_LOG",
    "INVALID_NODE",
    "INVALID_BUFFER",
    "WRITE_BUF_ERR",
    "INVALID_ROOT",
    "EMPTY_TREE_DEL",
    "INVALID_SIZE",
    "INVALID_VALUE",
    "INVALID_HASH",
    "INVALID_CHILDS",
    "INVALID_DB_PTR"
};

#endif //_TREE_H_

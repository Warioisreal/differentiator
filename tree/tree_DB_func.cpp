#include "checkers.h"

#include "tree_DB_func.h"


static tree_return_t TreeGetDB(Tree_type* tree, char* buffer, Node_t* node, int* position);
static Node_t* TreeReadDBRec(Tree_type* tree, char* buffer, int* position);
static char* ReadData(char* buffer, int* count, int* position);

static void SkipSpaces(char* buffer, int* position);

tree_return_t TreeMakeDB(const char* filename, Tree_type* tree) {
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

tree_return_t TreeReadDB(const char* filename, Tree_type* tree) {
    char buffer[MAX_BUFFER_DB_SIZE] = "";

    FILE* file = fopen(filename, "rb");
    fread(buffer, sizeof(char), MAX_BUFFER_DB_SIZE, file);
    fclose(file);

    int position = 0;
    tree->size = 0;

    TreeDtorRec(&(tree->root), &(tree->size));

    Node_t* node = TreeReadDBRec(tree, buffer, &position);

    if (node == nullptr) {
        TreeDump(tree, "DB READ CREATE ROOT ERROR", tree_return_t::INVALID_ROOT);
        return tree_return_t::INVALID_ROOT;
    }
    tree->root = node;

    //TREE_VERIFY_AND_RETURN(tree, tree->root, true, "ERROR AFTER ReadDB");

    TreePrint(tree, "DUMP DB TREE");

    SubTreeFillGrey(tree->root);

    return tree_return_t::TREE_OK;
}

static Node_t* TreeReadDBRec(Tree_type* tree, char* buffer, int* position) {
    if (buffer[*position] == '(') {
        (*position)++; // skip (
        if(isspace(buffer[*position]) == true) { SkipSpaces(buffer, position); }

        int count = 0;
        Node_t* node = nullptr;
        union ValueData value;
        const char* read_value = ReadData(buffer, &count, position);

        if (IsDouble(read_value) == true) {
            value.number = atof(read_value);
            node = MakeTreeElement(node_type::NUMBER, value);
        } else {
            size_t read_value_hash = CalculateStringHash(read_value);
            if (IsVariable(read_value_hash) == true) {
                value.variable = read_value;
                node = MakeTreeElement(node_type::VARIABLE, value);
            } else {
                operation_type opr = GetTypeOperation(read_value_hash);
                if (opr != operation_type::DEFAULT) {
                    value.operation = opr;
                    node = MakeTreeElement(node_type::OPERATION, value);
                }
            }
        }
        tree->size++;
        *position += count + 1;
        if(isspace(buffer[*position]) == true) { SkipSpaces(buffer, position); }
        node->left  = TreeReadDBRec(tree, buffer, position);
        if(isspace(buffer[*position]) == true) { SkipSpaces(buffer, position); }
        node->right = TreeReadDBRec(tree, buffer, position);
        if(isspace(buffer[*position]) == true) { SkipSpaces(buffer, position); }
        (*position)++; // skip )
        if(isspace(buffer[*position]) == true) { SkipSpaces(buffer, position); }

        node->hash = CalculateNodeHash(node);
        return node;
    }
    if (buffer[*position] == 'n') {
        *position += 3;
        return nullptr;
    }
    return nullptr;
}

static char* ReadData(char* buffer, int* count, int* position) {
    sscanf(buffer + *position, "%*s%n", count);
    *(buffer + *position + *count) = '\0';
    return buffer + *position;

}

//----------------------------------------------------------------------------------

static void SkipSpaces(char* buffer, int* position) {
    while (isspace(buffer[*position]) == true) {
        (*position)++;
    }
}

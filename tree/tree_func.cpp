#include "color_lib.h"
#include "stack/stack_lib.h"

#include "tree_func.h"


tree_return_t TreeCtor(Tree_type* tree) {
    #ifdef LOG_TREE
    FILE* log_file_ = nullptr;
    StartLog(&log_file_, tree->log->name);
    if (log_file_ == nullptr) {
        TreeDump(tree, "LOG FILE OPEN ERROR", tree_return_t::INVALID_LOG);
        return tree_return_t::INVALID_LOG;
    }
    tree->log->file_log   = log_file_;
    tree->log->dump_count = 1;
    #endif

    union ValueData value;
    value.operation = operation_type::DEFAULT;
    tree->root = MakeTreeElement(node_type::DEFAULT, value);
    tree->size = 1;

    //TREE_VERIFY_AND_RETURN(tree, tree->root, true, "ERROR IN Ctor");

    TreePrint(tree, "Ctor");

    SubTreeFillGrey(tree->root);

    return tree_return_t::TREE_OK;
}

//----------------------------------------------------------------------------------

tree_return_t TreeDtor(Tree_type* tree) {
    //TREE_VERIFY_AND_RETURN(tree, tree->root, false, "ERROR IN Dtor");

    TreePrint(tree, "Dtor");
    #ifdef LOG_TREE
    FinishLog(&(tree->log->file_log));
    #endif
    return TreeDtorRec(&(tree->root), &(tree->size));
}

tree_return_t TreeDtorRec(Node_t** node, size_t* size) {
    if (*node == nullptr) {
        printf("NODE POINTER ERROR IN TreeDtorRec\n");
        return tree_return_t::INVALID_NODE;
    }

    if ((*node)->left != nullptr) {
        TreeDtorRec(&((*node)->left), size);
    }
    if ((*node)->right != nullptr) {
        TreeDtorRec(&((*node)->right), size);
    }

    free(*node);
    *node = nullptr;
    (*size)--;
    return tree_return_t::TREE_OK;
}

//----------------------------------------------------------------------------------

void TreePrint(Tree_type* tree, const char* message) {
    SubTreePrint(tree, tree->root, message);
}

void SubTreePrint(Tree_type* tree, Node_t* node, const char* message) {
    SubTreeDump(tree, node, message, tree_return_t::TREE_OK);
}

void TreeDump(Tree_type* tree, const char* message, tree_return_t error) {
    SubTreeDump(tree, tree->root, message, error);
}

void SubTreeDump(Tree_type* tree, Node_t* node, const char* message, tree_return_t error) {
    if (error != tree_return_t::TREE_OK) {
        PRINT_COLOR(CYAN, "\n============TREE DUMP=============\n");
        PRINT_COLOR_VAR(RED, "ERROR: %s\n", TreeErrorsArray[static_cast <int>(error)]);
        PRINT_COLOR_VAR(RED, "message: %s\n", message);
    }
    switch(error) {
        case tree_return_t::TREE_OK:
        case tree_return_t::INVALID_ANSWER:
        case tree_return_t::INCORRECT_ANSW:
            #ifdef LOG_TREE
            GoLog(tree->root, tree->size, node, message, tree->log);
            #endif
            break;
        case tree_return_t::INVALID_SIZE:
            #ifdef LOG_TREE
            printf("| ___ROOT___ | ___SIZE___ | ___LOG___ |\n");
            #else
            printf("| ___ROOT___ | ___SIZE___ |\n");
            #endif
            printf("|");
            PRINT_COLOR_VAR(GREEN, " %10zX ", (size_t)tree->root);
            printf("|");
            PRINT_COLOR_VAR(RED, " %10zu ", tree->size);
            printf("|");
            #ifdef LOG_TREE
            PRINT_COLOR_VAR(GREEN, " %9zX ", (size_t)tree->log);
            printf("|");
            #endif
            printf("\n\n");
            #ifdef LOG_TREE
            printf("DUMP NUMBER: %zu\n", tree->log->dump_count);
            GoLog(tree->root, tree->size, node, message, tree->log);
            #endif
            break;
        case tree_return_t::INVALID_LOG:
            #ifdef LOG_TREE
            printf("| ___ROOT___ | ___SIZE___ | ___LOG___ |\n");
            #else
            printf("| ___ROOT___ | ___SIZE___ |\n");
            #endif
            printf("|");
            PRINT_COLOR_VAR(GREEN, " %10zX ", (size_t)tree->root);
            printf("|");
            PRINT_COLOR_VAR(GREEN, " %10zu ", tree->size);
            printf("|");
            #ifdef LOG_TREE
            PRINT_COLOR_VAR(GREEN, " %9zX ", (size_t)tree->log);
            printf("|");
            #endif
            printf("\n\n");
            #ifdef LOG_TREE
            printf("DUMP NUMBER: %zu\n", tree->log->dump_count);
            GoLog(tree->root, tree->size, node, message, tree->log);
            #endif
            break;
        case tree_return_t::INVALID_ROOT:
            #ifdef LOG_TREE
            printf("| ___ROOT___ | ___SIZE___ | ___LOG___ |\n");
            #else
            printf("| ___ROOT___ | ___SIZE___ |\n");
            #endif
            printf("|");
            PRINT_COLOR_VAR(RED, " %10zX ", (size_t)tree->root);
            printf("|");
            PRINT_COLOR_VAR(GREEN, " %10zu ", tree->size);
            printf("|");
            #ifdef LOG_TREE
            PRINT_COLOR_VAR(GREEN, " %9zX ", (size_t)tree->log);
            printf("|");
            #endif
            printf("\n\n");
            #ifdef LOG_TREE
            printf("DUMP NUMBER: %zu\n", tree->log->dump_count);
            GoLog(tree->root, tree->size, node, message, tree->log);
            #endif
            break;
        case tree_return_t::ADD_ELEMENT:
        case tree_return_t::DELETE_SUBTREE:
            #ifdef LOG_TREE
            printf("| ___ROOT___ | ___SIZE___ | ___LOG___ | ______UPD NODE______ |\n");
            #else
            printf("| ___ROOT___ | ___SIZE___ | ______UPD NODE______ |\n");
            #endif
            printf("|");
            PRINT_COLOR_VAR(GREEN, " %10zX ", (size_t)tree->root);
            printf("|");
            PRINT_COLOR_VAR(GREEN, " %10zu ", tree->size);
            printf("|");
            #ifdef LOG_TREE
            PRINT_COLOR_VAR(GREEN, " %9zX ", (size_t)tree->log);
            printf("|");
            #endif
            switch (node->type) {
                case node_type::DEFAULT:
                case node_type::OPERATION:
                    PRINT_COLOR_VAR(YELLOW, " %20s ", OprTable[(int)(node->value.operation)].name);
                    break;
                case node_type::VARIABLE:
                    PRINT_COLOR_VAR(YELLOW, " %20s ", node->value.variable);
                    break;
                case node_type::NUMBER:
                    PRINT_COLOR_VAR(YELLOW, " %20lg ", node->value.number);
                    break;
                default: PRINT_COLOR(RED, "unknown node_type\n");

            };
            printf("|");
            printf("\n\n");
            #ifdef LOG_TREE
            printf("DUMP NUMBER: %zu\n", tree->log->dump_count);
            GoLog(tree->root, tree->size, node, message, tree->log);
            #endif
            break;
        case tree_return_t::INVALID_CHILDS:
        case tree_return_t::INVALID_HASH:
        case tree_return_t::INVALID_VALUE:
            #ifdef LOG_TREE
            printf("| ___ROOT___ | ___SIZE___ | ___LOG___ | ___INCORRECT NODE___ |\n");
            #else
            printf("| ___ROOT___ | ___SIZE___ | ___INCORRECT NODE___ |\n");
            #endif
            printf("|");
            PRINT_COLOR_VAR(GREEN, " %10zX ", (size_t)tree->root);
            printf("|");
            PRINT_COLOR_VAR(GREEN, " %10zu ", tree->size);
            printf("|");
            #ifdef LOG_TREE
            PRINT_COLOR_VAR(GREEN, " %9zX ", (size_t)tree->log);
            printf("|");
            #endif
            switch (node->type) {
                case node_type::DEFAULT:
                case node_type::OPERATION:
                    PRINT_COLOR_VAR(YELLOW, " %20s ", OprTable[(int)(node->value.operation)].name);
                    break;
                case node_type::VARIABLE:
                    PRINT_COLOR_VAR(YELLOW, " %20s ", node->value.variable);
                    break;
                case node_type::NUMBER:
                    PRINT_COLOR_VAR(YELLOW, " %20lg ", node->value.number);
                    break;
                default: PRINT_COLOR(RED, "unknown node_type\n");

            };
            printf("|");
            printf("\n\n");
            #ifdef LOG_TREE
            printf("DUMP NUMBER: %zu\n", tree->log->dump_count);
            GoLog(tree->root, tree->size, node, message, tree->log);
            #endif
            break;
        case tree_return_t::INVALID_NODE:
            #ifdef LOG_TREE
            printf("| ___ROOT___ | ___SIZE___ | ___LOG___ | ___INCORRECT NODE___ |\n");
            #else
            printf("| ___ROOT___ | ___SIZE___ | ___INCORRECT NODE___ |\n");
            #endif
            printf("|");
            PRINT_COLOR_VAR(GREEN, " %10zX ", (size_t)tree->root);
            printf("|");
            PRINT_COLOR_VAR(GREEN, " %10zu ", tree->size);
            printf("|");
            #ifdef LOG_TREE
            PRINT_COLOR_VAR(GREEN, " %9zX ", (size_t)tree->log);
            printf("|");
            #endif
            PRINT_COLOR_VAR(RED, " %10zX ", (size_t)node);
            printf("|");
            printf("\n\n");
            #ifdef LOG_TREE
            printf("DUMP NUMBER: %zu\n", tree->log->dump_count);
            GoLog(tree->root, tree->size, node, message, tree->log);
            #endif
            break;
        case tree_return_t::INVALID_BUFFER:
            PRINT_COLOR(RED, "Ошибка буфера в работе с BD\n");
            break;
        case tree_return_t::WRITE_BUF_ERR:
            PRINT_COLOR(RED, "Ошибка записи в буфер\n");
            break;
        case tree_return_t::INVALID_DB_PTR:
            PRINT_COLOR(RED, "Ошибка аллокации буфера\n");
            break;
        default:
            printf("UNKNOWN RETURN\n");
    }

    if (error != tree_return_t::TREE_OK) { PRINT_COLOR(CYAN, "==================================\n\n"); }
}

//----------------------------------------------------------------------------------

tree_return_t TreeVerify(Tree_type* tree) {
    if (tree->root == nullptr) {
        return tree_return_t::INVALID_ROOT;
    }

    size_t count = 0;

    TreeCountNodes(tree->root, &count);
    if (count != tree->size) {
        return tree_return_t::INVALID_SIZE;
    }

    return tree_return_t::TREE_OK;
}

//----------------------------------------------------------------------------------

tree_return_t SubTreeVerify(Node_t* node) {
    if (node == nullptr) {
        return tree_return_t::INVALID_VALUE;
    }
    if (node->hash != CalculateNodeHash(node)) {
        return tree_return_t::INVALID_HASH;
    }

    return tree_return_t::TREE_OK;
}

//----------------------------------------------------------------------------------

void TreeCountNodes(Node_t* node, size_t* count) {
    if (node->left != nullptr) {
        TreeCountNodes(node->left, count);
    }
    if (node->right != nullptr) {
        TreeCountNodes(node->right, count);
    }
    (*count)++;
}

#include "tree_func.h"


static void TreeVisitPostOrder(Node_t* node, size_t* count);


tree_return_t TreeCtor(Tree_type* tree) {
    FILE* file_ = nullptr;
    StartLog(&file_, tree->log->name);
    if (file_ == nullptr) {
        TreeDump(tree, "LOG FILE OPEN ERROR", tree_return_t::INVALID_LOG);
        return tree_return_t::INVALID_LOG;
    }
    tree->log->file_log   = file_;
    tree->log->dump_count = 1;

    union ValueData value;
    value.operation = "CTOR";
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

    FinishLog(&(tree->log->file_log));
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
    /*
    if ((*node)->allocated_node == true) {
        free(const_cast<char*>((*node)->value));
    }*/
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
    }

    GoLog(tree->root, tree->size, node, message, tree->log);
    /*switch(error) {
        case tree_return_t::TREE_OK:
        case tree_return_t::INVALID_ANSWER:
        case tree_return_t::INCORRECT_ANSW:
            GoLog(tree->root, tree->size, node, message, tree->log);
            break;
        case tree_return_t::INVALID_SIZE:
            printf("| ___ROOT___ | ___SIZE___ | ___LOG___ |\n");
            printf("|");
            PRINT_COLOR_VAR(GREEN, " %10zX ", (size_t)tree->root);
            printf("|");
            PRINT_COLOR_VAR(RED, " %10zu ", tree->size);
            printf("|");
            PRINT_COLOR_VAR(GREEN, " %9zX ", (size_t)tree->log);
            printf("|");
            printf("\n\n");
            printf("DUMP NUMBER: %zu\n", tree->log->dump_count);
            GoLog(tree->root, tree->size, node, message, tree->log);
            break;
        case tree_return_t::INVALID_LOG:
            printf("| ___ROOT___ | ___SIZE___ | ___LOG___ |\n");
            printf("|");
            PRINT_COLOR_VAR(GREEN, " %10zX ", (size_t)tree->root);
            printf("|");
            PRINT_COLOR_VAR(GREEN, " %10zu ", tree->size);
            printf("|");
            PRINT_COLOR_VAR(RED, " %9zX ", (size_t)tree->log);
            printf("|");
            printf("\n\n");
            printf("DUMP NUMBER: %zu\n", tree->log->dump_count);
            GoLog(tree->root, tree->size, node, message, tree->log);
            break;
        case tree_return_t::INVALID_ROOT:
            printf("| ___ROOT___ | ___SIZE___ | ___LOG___ |\n");
            printf("|");
            PRINT_COLOR_VAR(RED, " %10zX ", (size_t)tree->root);
            printf("|");
            PRINT_COLOR_VAR(GREEN, " %10zu ", tree->size);
            printf("|");
            PRINT_COLOR_VAR(GREEN, " %9zX ", (size_t)tree->log);
            printf("|");
            printf("\n\n");
            printf("DUMP NUMBER: %zu\n", tree->log->dump_count);
            GoLog(tree->root, tree->size, node, message, tree->log);
            break;
        case tree_return_t::ADD_ELEMENT:
        case tree_return_t::DELETE_SUBTREE:
            printf("| ___ROOT___ | ___SIZE___ | ___LOG___ | ______UPD NODE______ |\n");
            printf("|");
            PRINT_COLOR_VAR(GREEN, " %10zX ", (size_t)tree->root);
            printf("|");
            PRINT_COLOR_VAR(GREEN, " %10zu ", tree->size);
            printf("|");
            PRINT_COLOR_VAR(GREEN, " %9zX ", (size_t)tree->log);
            printf("|");
            PRINT_COLOR_VAR(YELLOW, " %20s ", node->value);
            printf("|");
            printf("\n\n");
            printf("DUMP NUMBER: %zu\n", tree->log->dump_count);
            GoLog(tree->root, tree->size, node, message, tree->log);
            break;
        case tree_return_t::INVALID_CHILDS:
        case tree_return_t::INVALID_HASH:
        case tree_return_t::INVALID_VALUE:
            printf("| ___ROOT___ | ___SIZE___ | ___LOG___ | ___INCORRECT NODE___ |\n");
            printf("|");
            PRINT_COLOR_VAR(GREEN, " %10zX ", (size_t)tree->root);
            printf("|");
            PRINT_COLOR_VAR(GREEN, " %10zu ", tree->size);
            printf("|");
            PRINT_COLOR_VAR(GREEN, " %9zX ", (size_t)tree->log);
            printf("|");
            PRINT_COLOR_VAR(RED, " %20s ", node->value);
            printf("|");
            printf("\n\n");
            printf("DUMP NUMBER: %zu\n", tree->log->dump_count);
            GoLog(tree->root, tree->size, node, message, tree->log);
            break;
        case tree_return_t::INVALID_NODE:
            printf("| ___ROOT___ | ___SIZE___ | ___LOG___ | ___INCORRECT NODE___ |\n");
            printf("|");
            PRINT_COLOR_VAR(GREEN, " %10zX ", (size_t)tree->root);
            printf("|");
            PRINT_COLOR_VAR(GREEN, " %10zu ", tree->size);
            printf("|");
            PRINT_COLOR_VAR(GREEN, " %9zX ", (size_t)tree->log);
            printf("|");
            PRINT_COLOR_VAR(RED, " %10zX ", (size_t)node);
            printf("|");
            printf("\n\n");
            printf("DUMP NUMBER: %zu\n", tree->log->dump_count);
            GoLog(tree->root, tree->size, node, message, tree->log);
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

    */if (error != tree_return_t::TREE_OK) { PRINT_COLOR(CYAN, "==================================\n\n"); }
}

//----------------------------------------------------------------------------------

tree_return_t TreeVerify(Tree_type* tree) {
    if (tree->root == nullptr) {
        return tree_return_t::INVALID_ROOT;
    }

    size_t count = 0;

    TreeVisitPostOrder(tree->root, &count);
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
    /*if (node->value == DATA_POISON) {
        MakeRedElem(node);
        return tree_return_t::INVALID_VALUE;
    }
    if (node->hash != CalculateNodeHash(node)) {
        return tree_return_t::INVALID_HASH;
    }
    if (node->correct_childs == true && ((node->left == nullptr) != (node->right == nullptr))) {
        MakeRedElem(node);
        return tree_return_t::INVALID_CHILDS;
    }*/
    return tree_return_t::TREE_OK;
}

//----------------------------------------------------------------------------------

static void TreeVisitPostOrder(Node_t* node, size_t* count) {
    if (node->left != nullptr) {
        TreeVisitPostOrder(node->left, count);
    }
    if (node->right != nullptr) {
        TreeVisitPostOrder(node->right, count);
    }
    (*count)++;
}

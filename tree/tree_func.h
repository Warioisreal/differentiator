#ifndef _TREE_FUNC_H_
#define _TREE_FUNC_H_


#include "define_lib.h"

#include "logger.h"

#include "tree_node_func.h"


struct Tree_type {
    Node_t* root = nullptr;
    size_t size = 0;
    LOG* log = nullptr;
};

#define MAKE_LOG(tree_name, log_name) \
    LOG log_name = {}; \
    tree_name.log = &(log_name); \
    snprintf(tree_name.log->name, LOG_FILE_NAME_SIZE, "%s", #tree_name)

#define MAKE_TREE(tree_name) \
    Tree_type tree_name = {}; \
    MAKE_LOG(tree_name, log); \
    char full_folder_name[LOG_FOLDER_NAME_SIZE] = ""; \
    GetFullFolderName(#tree_name, full_folder_name); \
    UpdateFolder(full_folder_name); \
    TreeCtor(&tree_name)

#define CHECK_ERROR_AND_RETURN(tree, message, ret, result) if (ret != result) { TreeDump(tree, message, ret); return ret; }

#define TREE_VERIFY_AND_RETURN(tree, node, check_size, message) BEGIN { \
    tree_return_t error = TreeVerify(tree, check_size); \
    if (error != tree_return_t::TREE_OK) { \
        SubTreeDump(tree, node, message, error); \
        return error; \
    } else { \
        error = SubTreeVerify(node); \
        if (error != tree_return_t::TREE_OK) { \
            SubTreeDump(tree, node, message, error); \
            return error; \
        } \
    } \
} END

tree_return_t TreeCtor(Tree_type* tree);
tree_return_t TreeDtor(Tree_type* tree);
tree_return_t TreeDtorRec(Node_t** node, size_t* size);

void TreePrint(Tree_type* tree, const char* message);
void SubTreePrint(Tree_type* tree, Node_t* node, const char* message);
void TreeDump(Tree_type* tree, const char* message, tree_return_t error);
void SubTreeDump(Tree_type* tree, Node_t* node, const char* message, tree_return_t error);

tree_return_t TreeVerify(Tree_type* tree, bool check_size);
tree_return_t SubTreeVerify(Node_t* node);
void TreeCountNodes(Node_t* node, size_t* count);

#endif //_TREE_FUNC_H_

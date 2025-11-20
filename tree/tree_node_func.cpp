#include <stdio.h>

#include "tree_node_func.h"


static size_t djb2(size_t hash, size_t field);


Node_t* MakeTreeElement(node_type type, union ValueData value) {
    Node_t* buf = (Node_t*)calloc(1, sizeof(Node_t));
    if (buf == nullptr) {
        printf("node allocate error\n");
        return buf;
    }

    buf->type = type;
    switch (type) {
        case node_type::DEFAULT:
        case node_type::OPERATION:
            buf->value.operation = value.operation;
            break;
        case node_type::VARIABLE:
            buf->value.variable = value.variable;
            break;
        case node_type::NUMBER:
            buf->value.number = value.number;
            break;
        default:
            printf("INVALID NODE TYPE\n");
            free(buf); buf = nullptr;
            return buf;
    }

    buf->left  = nullptr;
    buf->right = nullptr;

    MakeGreenElem(buf);

    return buf;
}

//----------------------------------------------------------------------------------

size_t CalculateNodeHash(Node_t* node) {
    if (node == nullptr) { return 0; }

    size_t struct_hash = HASH_SEED;

    struct_hash = djb2(struct_hash, (size_t)node->type);
    struct_hash = djb2(struct_hash, (size_t)node->value.number);
    struct_hash = djb2(struct_hash, (size_t)node->left);
    struct_hash = djb2(struct_hash, (size_t)node->right);
    struct_hash = djb2(struct_hash, (size_t)node->color);
    struct_hash = djb2(struct_hash, (size_t)node->bg_color);

    return struct_hash;
}

//----------------------------------------------------------------------------------

void SubTreeFillGrey(Node_t* node) {
    if (node == nullptr) {printf("NULL\n"); return; }

    if (node->left != nullptr) {
        SubTreeFillGrey(node->left);
    }
    if (node->right != nullptr) {
        SubTreeFillGrey(node->right);
    }
    MakeGreyElem(node);
}

void MakeGreenElem(Node_t* node) {
    if (node == nullptr) {printf("NULL\n"); return; }
    node->color    = 0x00c000;
    node->bg_color = 0xa0f0a0;

    node->hash = CalculateNodeHash(node);
}

void MakeYellowElem(Node_t* node) {
    if (node == nullptr) {printf("NULL\n"); return; }
    node->color    = 0xc0c000;
    node->bg_color = 0xf0f0a0;

    node->hash = CalculateNodeHash(node);
}

void MakeRedElem(Node_t* node) {
    if (node == nullptr) {printf("NULL\n"); return; }
    node->color    = 0xc00000;
    node->bg_color = 0xf0a0a0;

    node->hash = CalculateNodeHash(node);
}

void MakeGreyElem(Node_t* node) {
    if (node == nullptr) {printf("NULL\n"); return; }
    node->color    = 0x808080;
    node->bg_color = 0xc0c0c0;

    node->hash = CalculateNodeHash(node);
}

//----------------------------------------------------------------------------------

static size_t djb2(size_t hash, size_t field) {
    return ((hash << 5) + hash) + field;
}

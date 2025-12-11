#include "tree/tree_func.h"
#include "tree/checkers.h"
#include "tree/latex.h"

#include "optimazer.h"


static void ReplaceParentOnChild(Node_t** parent, Node_t* child);


dfr_return_t OptimizeTree(Tree_type* tree, LATEX* latex) {
    DFR_VERIFY_AND_RETURN(tree, tree->root, "error before optimization");
    dfr_return_t result = dfr_return_t::OK;

    TechBeginSubsubsection(latex, "Бесполезное упрощение уравнения");
    TechAppendText(latex, "Итак, вот с чем мы работаем:");
    TechBeginEquationBlock(latex, "");
    TechAppendFormula(latex, tree->root);
    TechEndEquationBlock(latex);

    bool is_update = true;
    while (is_update == true) {
        is_update = false;
        CHECK_AND_RETURN(OptimizeConstantElements(tree, &tree->root, &is_update));

        TechBeginEquationBlock(latex, "");
        TechAppendFormula(latex, tree->root);
        TechEndEquationBlock(latex);

        CHECK_AND_RETURN(DeleteNeutralElements(tree, &tree->root, &is_update));

        TechBeginEquationBlock(latex, "");
        TechAppendFormula(latex, tree->root);
        TechEndEquationBlock(latex);
    }
    TechBeginEquationBlock(latex, "Выражение после упрощения");
    TechAppendFormula(latex, tree->root);
    TechEndEquationBlock(latex);

    TreePrint(tree, "Optimize tree");

    SubTreeFillGrey(tree->root);

    DFR_VERIFY_AND_RETURN(tree, tree->root, "error after optimization");

    return dfr_return_t::OK;
}


#define T_ (*node)->type
#define V_ (*node)->value
#define OP_ (*node)->value.operation
#define L_ (*node)->left
#define R_ (*node)->right
#define LT_ (*node)->left->type
#define RT_ (*node)->right->type
#define LV_ (*node)->left->value
#define RV_ (*node)->right->value
#define LN_ (*node)->left->value.number
#define RN_ (*node)->right->value.number

#define MAKE_NUMBER_NODE \
    TreeDtorRec(node, &(tree->size)); \
    *node = MakeTreeElement(node_type::NUMBER, value); \
    tree->size++; \
    *is_update = true;


dfr_return_t OptimizeConstantElements(Tree_type* tree, Node_t** node, bool* is_update) {
    dfr_return_t result = dfr_return_t::OK;
    if (*node == nullptr) { return dfr_return_t::NODE_PTR_ERROR; }

    if (T_ == node_type::OPERATION) {
        if (L_ != nullptr) { CHECK_AND_RETURN(OptimizeConstantElements(tree, &(L_), is_update)); }
        if (R_ != nullptr) { CHECK_AND_RETURN(OptimizeConstantElements(tree, &(R_), is_update)); }

        if ((L_ != nullptr) && (R_ != nullptr)) {
            if (LT_ == node_type::NUMBER && RT_ == node_type::NUMBER) {
                union ValueData value;
                value.number = SolveRec(*node);
                MAKE_NUMBER_NODE;
            }
        }
    }

    return dfr_return_t::OK;
}

dfr_return_t DeleteNeutralElements(Tree_type* tree, Node_t** node, bool* is_update) {
    dfr_return_t result = dfr_return_t::OK;
    if (*node == nullptr) { return dfr_return_t::NODE_PTR_ERROR; }

    if (T_ == node_type::OPERATION) {
        if (L_ != nullptr) { CHECK_AND_RETURN(DeleteNeutralElements(tree, &L_, is_update)); }
        if (R_ != nullptr) { CHECK_AND_RETURN(DeleteNeutralElements(tree, &R_, is_update)); }

        if (OP_ == operation_type::MUL) {
            if ((LT_ == node_type::NUMBER && CompareDouble(LN_, 0) == 0) || \
                (RT_ == node_type::NUMBER && CompareDouble(RN_, 0) == 0)) {
                union ValueData value;
                value.number = 0;
                MAKE_NUMBER_NODE;
            } else
            if (LT_ == node_type::NUMBER && CompareDouble(LN_, 1) == 0) {
                TreeDtorRec(&L_, &(tree->size));
                ReplaceParentOnChild(node, R_);
                tree->size--;
            } else
            if (RT_ == node_type::NUMBER && CompareDouble(RN_, 1) == 0) {
                TreeDtorRec(&R_, &(tree->size));
                ReplaceParentOnChild(node, L_);
                tree->size--;
            }
        } else
        if (OP_ == operation_type::ADD) {
            if (LT_ == node_type::NUMBER && CompareDouble(LN_, 0) == 0) {
                TreeDtorRec(&L_, &(tree->size));
                ReplaceParentOnChild(node, R_);
                tree->size--;
            } else
            if (RT_ == node_type::NUMBER && CompareDouble(RN_, 0) == 0) {
                TreeDtorRec(&R_, &(tree->size));
                ReplaceParentOnChild(node, L_);
                tree->size--;
            }
        } else
        if (OP_ == operation_type::SUB) {
            if (RT_ == node_type::NUMBER && CompareDouble(RN_, 0) == 0) {
                TreeDtorRec(&R_, &(tree->size));
                ReplaceParentOnChild(node, L_);
                tree->size--;
            }
        } else
        if (OP_ == operation_type::DIV) {
            if (LT_ == node_type::NUMBER && CompareDouble(LN_, 0) == 0) {
                union ValueData value;
                value.number = 0;
                MAKE_NUMBER_NODE;
            } else
            if (RT_ == node_type::NUMBER && CompareDouble(RN_, 1) == 0) {
                TreeDtorRec(&R_, &(tree->size));
                ReplaceParentOnChild(node, L_);
                tree->size--;
            }
        } else
        if (OP_ == operation_type::POW) {
            if (RT_ == node_type::NUMBER && CompareDouble(RN_, 0) == 0) {
                union ValueData value;
                value.number = 1;
                MAKE_NUMBER_NODE;
            } else
            if (RT_ == node_type::NUMBER && CompareDouble(RN_, 1) == 0) {
                TreeDtorRec(&R_, &(tree->size));
                ReplaceParentOnChild(node, L_);
                tree->size--;
            }
        }
    }

    return dfr_return_t::OK;
}

static void ReplaceParentOnChild(Node_t** parent, Node_t* child) {
    free(*parent);
    *parent = child;
    MakeYellowElem(*parent);
}

#undef T_
#undef V_
#undef OP_
#undef L_
#undef R_
#undef LT_
#undef RT_
#undef LV_
#undef RV_
#undef LN_
#undef RN_
#undef MAKE_NUMBER_NODE
#undef COPY_NODE

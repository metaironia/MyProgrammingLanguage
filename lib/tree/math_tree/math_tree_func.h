#ifndef MATH_TREE_FUNC_H
#define MATH_TREE_FUNC_H

#include "math_tree_node_data.h"
#include "../tree_func.h"

//TODO put below to dsl_commands

#define IS_VALUE_EQUAL(val, branch)   (current_node -> branch -> data -> nodeType == NUMBER &&                             \
                                      IsDoublesEqual ((current_node -> branch -> data -> nodeValue).mathNodeValue, val))

#define NODE_VALUE                    ((current_node -> data -> nodeValue).mathNodeValue)

#define NODE_TYPE                     (current_node -> data -> nodeType)

#define NODE_MATH_OPERATOR            ((current_node -> data -> nodeValue).mathOperator)

#define NODE_LANG_OPERATOR            ((current_node -> data -> nodeValue).langOperator)

#define BRANCH_VALUE(branch)          ((current_node -> branch -> data -> nodeValue).mathNodeValue)

#define BRANCH_TYPE(branch)           ((current_node -> branch -> data -> nodeType))

#define BRANCH_OPERATOR(branch)       ((current_node -> branch -> data -> nodeValue).mathOperator)



#define MATH_TREE_VERIFY(math_tree, func_type)                                                     \
                                {                                                       \
                                    if (MathTreeVerify (math_tree, __func__) != 0) {    \
                                                                                        \
                                        MathTreeGraphDump (math_tree);                  \
                                                                                        \
                                        return func_type##_FUNC_STATUS_FAIL;                   \
                                    }                                                   \
                                }

#define MATH_TREE_VERIFY_PTR_FUNC(math_tree)                                            \
                                {                                                       \
                                    if (MathTreeVerify (math_tree, __func__) != 0) {    \
                                                                                        \
                                        MathTreeGraphDump (math_tree);                  \
                                                                                        \
                                        return NULL;                                    \
                                    }                                                   \
                                }

#define MATH_TREE_NODE_VERIFY(math_tree_node, func_type)                            \
                                {                                                   \
                                    if (MathTreeNodeVerify (math_tree_node) != 0) { \
                                                                                    \
                                        return func_type##_FUNC_STATUS_FAIL;        \
                                    }                                               \
                                }

#define MATH_TREE_NODE_VERIFY_PTR_FUNC(math_tree_node)                                  \
                                {                                                       \
                                    if (MathTreeNodeVerify (math_tree_node) != 0) {     \
                                                                                        \
                                        return NULL;                                    \
                                    }                                                   \
                                }

const int MAX_NUMBER_LENGTH = 30;

MathNodeType IsOperatorUnaryOrBinary (const MathNodeOperator node_operator_to_check);

TreeNode *CreateMathTreeNode (const MathNodeType type_of_node, const double node_value,
                              TreeNode *const ptr_left_branch,
                              TreeNode *const ptr_right_branch);

unsigned int MathTreeNodeVerify (const TreeNode *math_expression_tree_node);

unsigned int MathTreeVerify (const Tree *math_expression_tree,
                             const char* name_parent_func);

unsigned int MathTreeNodeChecker (const TreeNode *math_expression_node);

unsigned int MathNodeTypeCheckError (const TreeNode *math_expression_node);

unsigned int NodeBinaryOperatorCheckErrors (const TreeNode *math_expression_node);

unsigned int NodeUnaryOperatorCheckErrors (const TreeNode *math_expression_node);

unsigned int NodeVariableCheckErrors (const TreeNode *math_expression_node);

unsigned int NodeNumberCheckErrors (const TreeNode *math_expression_node);

const char *NumberToString (const double number);

const char *MathNodeTypeToString (const TreeNode *math_tree_node);

const char *MathNodeNumOrVarToString (const TreeNode *math_tree_node);

const char *MathNodeOperatorToString (const TreeNode *math_tree_node);

TreeFuncStatus MathTreeNodeConstantsSimplify (TreeNode *math_expression_node);

TreeFuncStatus MathTreeNodeBinaryOperatorSimplify (TreeNode *math_expression_node);

TreeFuncStatus MathTreeNodeNumAndNumSimplify (TreeNode *node_for_simplify);

TreeFuncStatus MathTreeNodeSmthAndZeroSimplify (TreeNode *node_for_simplify);

TreeFuncStatus MathTreeNodeSmthAndOneSimplify (TreeNode *node_for_simplify);

double MathTreeCompute (const Tree *math_expression_tree, const double variable_value);

double MathTreeNodeCompute (const TreeNode *math_tree_node, const double variable_value);

double MathTreeNodeComputeOperatorResult (const TreeNode *math_tree_node,
                                          const double left_branch_node_value,
                                          const double right_branch_node_value,
                                          const double variable_value);

double MathTreeNodeUnaryCompute (const double left_branch_value,
                                 const MathNodeOperator current_node_operator);

double MathTreeNodeBinaryCompute (const double left_branch_value, const double right_branch_value,
                                  const MathNodeOperator current_node_operator);

//---------------------------------------------------------------------------------------------

TreeFuncStatus MathTreeNodeRead (FILE *file_for_read_tree, TreeNode **tree_node_for_fill);

TreeFuncStatus MathTreeNodeNilCheck (FILE *file_for_node_nil_check, char *buffer_for_node_check);

TreeFuncStatus MathTreeNodeDataRead (FILE *file_for_read_node_data, TreeNode **tree_node_for_data_read,
                                     char *buffer_for_read_node_data);


#endif

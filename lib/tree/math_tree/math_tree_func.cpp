#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "dsl_commands.h"

#include "../tree_func.h"
#include "../tree_log.h"

#include "math_tree_func.h"
#include "math_tree_node_data.h"

#include "math_operation.h"

TreeNode *CreateMathTreeNode (const MathNodeType type_of_node, const double node_value,
                              TreeNode *const ptr_left_branch,
                              TreeNode *const ptr_right_branch) {

    TreeNode *current_node = CreateTreeNode ();
    assert (current_node);

    current_node -> data = (MathNode *) calloc (1, sizeof (MathNode));
    assert (current_node -> data);

    if ((type_of_node != NUMBER && type_of_node != VARIABLE) &&
        IsOperatorUnaryOrBinary ((MathNodeOperator) node_value) != type_of_node)

        return NULL;

    current_node -> data -> nodeType = type_of_node;

    if (type_of_node == NUMBER || type_of_node == VARIABLE)
        NODE_VALUE = node_value;

    else
        NODE_MATH_OPERATOR = (MathNodeOperator) node_value;

    current_node -> left_branch  = ptr_left_branch;
    current_node -> right_branch = ptr_right_branch;

    return current_node;
}

MathNodeType IsOperatorUnaryOrBinary (const MathNodeOperator node_operator_to_check) {

    switch (node_operator_to_check) {

        case OPERATOR_ADD:
        case OPERATOR_SUB:
        case OPERATOR_DIV:
        case OPERATOR_MUL:
        case OPERATOR_POW:
            return BINARY_OPERATOR;
            break;

        case OPERATOR_SIN:
        case OPERATOR_COS:
        case OPERATOR_LN:
            return UNARY_OPERATOR;
            break;

        default:
            return NODE_TYPE_ERROR;
    }

    return NODE_TYPE_ERROR;
}

TreeFuncStatus MathNodeTypePrint (FILE *file_for_print,
                                  const TreeNode *current_node_for_print) {

    assert (file_for_print);
    assert (current_node_for_print);

    LOG_PRINT (file_for_print, "%s", MathNodeTypeToString (current_node_for_print));

    return TREE_FUNC_STATUS_OK;
}

const char *MathNodeTypeToString (const TreeNode *current_node) {

    assert (current_node);

    const char *node_type_to_string = NULL;

    if (!(node_type_to_string = MathNodeNumOrVarToString (current_node)))
        node_type_to_string = MathNodeOperatorToString (current_node);

    return node_type_to_string;
}

const char *MathNodeNumOrVarToString (const TreeNode *current_node) {

    assert (current_node);

    const MathNodeType node_type = NODE_TYPE;

    switch (node_type) {

        case NUMBER:
            return NumberToString (NODE_VALUE);
            break;

        case VARIABLE:
            return "x";
            break;

        default:
            return NULL;
    }

    return NULL;
}

const char *NumberToString (const double number) {

    static char number_to_string[MAX_NUMBER_LENGTH + 1] = {};

    memset (number_to_string, 0, MAX_NUMBER_LENGTH + 1);

    sprintf (number_to_string, "%.3lg", number);

    return number_to_string;
}

const char *MathNodeOperatorToString (const TreeNode *current_node) {

    assert (current_node);

    switch (NODE_MATH_OPERATOR) {

        case OPERATOR_ADD:
            return "+";
            break;

        case OPERATOR_SUB:
            return "-";
            break;

        case OPERATOR_DIV:
            return "/";
            break;

        case OPERATOR_MUL:
            return "*";
            break;

        case OPERATOR_POW:
            return "^";
            break;

        case OPERATOR_SIN:
            return "sin";
            break;

        case OPERATOR_COS:
            return "cos";
            break;

        case OPERATOR_LN:
            return "ln";
            break;

        case OPERATOR_GREATER:
            return ">";
            break;

        case OPERATOR_LESS:
            return "<";
            break;

        case OPERATOR_EQUAL:
            return "=";
            break;
        
        case OPERATOR_NOT_EQUAL:
            return "!=";
            break;

        case OPERATOR_OPEN_PARENTHESIS:
            return ")";
            break;

        case OPERATOR_CLOSE_PARENTHESIS:
            return ")";
            break;

        default:
            return NULL;
    }

    return NULL;
}

double MathTreeCompute (const Tree *math_expression_tree, const double variable_value) {

    if (MathTreeVerify (math_expression_tree, __func__) != 0)
        return NAN;

    return MathTreeNodeCompute (math_expression_tree -> root, variable_value);
}

double MathTreeNodeCompute (const TreeNode *current_node, const double variable_value) {

    if (!current_node)
        return 0;

    if (MathTreeNodeVerify (current_node) != 0)
        return NAN;

    const double left_branch_value  = MathTreeNodeCompute (current_node -> left_branch,
                                                           variable_value);

    const double right_branch_value = MathTreeNodeCompute (current_node -> right_branch,
                                                           variable_value);

    return MathTreeNodeComputeOperatorResult (current_node, left_branch_value, right_branch_value,
                                              variable_value);
}

double MathTreeNodeComputeOperatorResult (const TreeNode *current_node,
                                          const double left_branch_node_value,
                                          const double right_branch_node_value,
                                          const double variable_value) {

    if (MathTreeNodeVerify (current_node) != 0)
        return NAN;

    switch (NODE_TYPE) {

        case NUMBER:
            return NODE_VALUE;

        case VARIABLE:
            return variable_value;

        case UNARY_OPERATOR:
            return MathTreeNodeUnaryCompute (BRANCH_VALUE (left_branch), NODE_MATH_OPERATOR);

        case BINARY_OPERATOR:
            return MathTreeNodeBinaryCompute (BRANCH_VALUE (left_branch), BRANCH_VALUE (right_branch),
                                              NODE_MATH_OPERATOR);

        case NODE_TYPE_ERROR:
        default:
            fprintf (stderr, "UNKNOWN ERROR WHILST COMPUTING VARIABLE OR NUMBER\n");
            return NAN;
    }

    return NAN;
}

double MathTreeNodeUnaryCompute (const double left_branch_value,
                                 const MathNodeOperator current_node_operator) {

    switch (current_node_operator) {

        case OPERATOR_LN:
            return log (left_branch_value);

        case OPERATOR_SIN:
            return sin (left_branch_value);

        case OPERATOR_COS:
            return cos (left_branch_value);

        default:
            break;
    }

    return NAN;
}

double MathTreeNodeBinaryCompute (const double left_branch_value, const double right_branch_value,
                                  const MathNodeOperator current_node_operator) {

    switch (current_node_operator) {

        case OPERATOR_ADD:
            return left_branch_value + right_branch_value;

        case OPERATOR_SUB:
            return left_branch_value - right_branch_value;

        case OPERATOR_MUL:
            return left_branch_value * right_branch_value;

         case OPERATOR_DIV:
            if (!IsZero (right_branch_value))
                return left_branch_value / right_branch_value;
            else
                break;

        case OPERATOR_POW:
            return pow (left_branch_value, right_branch_value);

        default:
            break;
    }

    return NAN;
}

unsigned int MathTreeVerify (const Tree *math_expression_tree,
                             const char* name_parent_func) {

    assert (name_parent_func);

    unsigned int errors_math_expression_tree = TreeVerify (math_expression_tree, name_parent_func);

    errors_math_expression_tree = MathTreeNodeVerify (math_expression_tree -> root);

    return errors_math_expression_tree;
}

unsigned int MathTreeNodeVerify (const TreeNode *math_expression_tree_node) {

    unsigned int errors_math_tree_node = TreeNodeVerify (math_expression_tree_node);

    if (errors_math_tree_node != 0)
        return errors_math_tree_node;

    errors_math_tree_node = MathTreeNodeChecker (math_expression_tree_node);

    return errors_math_tree_node;
}

unsigned int MathTreeNodeChecker (const TreeNode *current_node) {

    if (!current_node)
        return 0;

    unsigned int errors_in_node_and_subtree = MathNodeTypeCheckError (current_node);

    if (errors_in_node_and_subtree)
        return errors_in_node_and_subtree;

    errors_in_node_and_subtree |= NodeBinaryOperatorCheckErrors (current_node) |
                                  NodeUnaryOperatorCheckErrors (current_node)  |
                                  NodeVariableCheckErrors (current_node)       |
                                  NodeNumberCheckErrors (current_node);

    errors_in_node_and_subtree |= MathTreeNodeChecker (current_node -> left_branch);
    errors_in_node_and_subtree |= MathTreeNodeChecker (current_node -> right_branch);

    return errors_in_node_and_subtree;
}

unsigned int MathNodeTypeCheckError (const TreeNode *current_node) {

    assert (current_node);

    unsigned int is_node_type_error = ((NODE_TYPE != NUMBER   &&
                                      NODE_TYPE != VARIABLE) &&
                                      IsOperatorUnaryOrBinary (NODE_MATH_OPERATOR) != NODE_TYPE);

    if (is_node_type_error)
        TREE_ERROR_SET_AND_PRINT (is_node_type_error, MATH_TREE_WRONG_NODE_TYPE);

    return is_node_type_error;
}

unsigned int NodeBinaryOperatorCheckErrors (const TreeNode *current_node) {

    assert (current_node);

    unsigned int is_binary_operator_error = 0;

    is_binary_operator_error = (NODE_TYPE == BINARY_OPERATOR &&
                               (!(current_node -> left_branch)                      ||
                               !(current_node -> right_branch)));

    if (is_binary_operator_error)
        TREE_ERROR_SET_AND_PRINT (is_binary_operator_error, MATH_TREE_BINARY_OP_WRONG_BRANCH);

    return is_binary_operator_error;
}

unsigned int NodeUnaryOperatorCheckErrors (const TreeNode *current_node) {

    assert (current_node);

    int is_unary_operator_error = 0;

    is_unary_operator_error = (NODE_TYPE == UNARY_OPERATOR &&
                              (((bool) (current_node -> left_branch))             ==
                              (bool) (current_node -> right_branch)));

    if (is_unary_operator_error)
        TREE_ERROR_SET_AND_PRINT (is_unary_operator_error, MATH_TREE_UNARY_OP_WRONG_BRANCHES);

    return is_unary_operator_error;
}

unsigned int NodeVariableCheckErrors (const TreeNode *current_node) {

    assert (current_node);

    unsigned int is_variable_error = (NODE_TYPE == VARIABLE &&
                                     (((current_node -> left_branch))              ||
                                     (current_node -> right_branch)));

    if (is_variable_error)
        TREE_ERROR_SET_AND_PRINT (is_variable_error, MATH_TREE_VARIABLE_HAVE_BRANCHES);

    return is_variable_error;
}

unsigned int NodeNumberCheckErrors (const TreeNode *current_node) {

    assert (current_node);

    unsigned int is_number_error (current_node -> data -> nodeType == NUMBER &&
                                 (((current_node -> left_branch))            ||
                                 (current_node -> right_branch)));

    if (is_number_error)
        TREE_ERROR_SET_AND_PRINT (is_number_error, MATH_TREE_NUMBER_HAVE_BRANCH);

    return is_number_error;
}

TreeFuncStatus MathTreeNodeConstantsSimplify (TreeNode *current_node) {

    MATH_TREE_NODE_VERIFY (current_node, TREE);

    if (!(current_node -> left_branch) &&
        !(current_node -> right_branch))

        return TREE_FUNC_STATUS_FAIL;

    if (MathTreeNodeConstantsSimplify (current_node -> left_branch) == TREE_FUNC_STATUS_OK)
        return TREE_FUNC_STATUS_OK;

    if (MathTreeNodeConstantsSimplify (current_node -> right_branch) == TREE_FUNC_STATUS_OK)
        return TREE_FUNC_STATUS_OK;

    switch (NODE_TYPE) {

        case UNARY_OPERATOR:
//            MathTreeNodeUnaryOperatorSimplify (current_node);
            break;

        case BINARY_OPERATOR:
            return MathTreeNodeBinaryOperatorSimplify (current_node);

        case NUMBER:
        case VARIABLE:
            return TREE_FUNC_STATUS_FAIL;

        case NODE_TYPE_ERROR:
        default:
            fprintf (stderr, "UNKNOWN NODE TYPE ERROR\n"); //TODO make output to log file
            return TREE_FUNC_STATUS_FAIL;
    }

    return TREE_FUNC_STATUS_FAIL;
}

TreeFuncStatus MathTreeNodeBinaryOperatorSimplify (TreeNode *current_node) {

    MATH_TREE_NODE_VERIFY (current_node, TREE);

    if (BRANCH_TYPE (left_branch) == NUMBER && BRANCH_TYPE (right_branch) == NUMBER)
        return MathTreeNodeNumAndNumSimplify (current_node);

    if (IS_VALUE_EQUAL (0, left_branch) || IS_VALUE_EQUAL (0, right_branch))
        return MathTreeNodeSmthAndZeroSimplify (current_node);

    if (IS_VALUE_EQUAL (1, left_branch) || IS_VALUE_EQUAL (1, right_branch)) {
        return MathTreeNodeSmthAndOneSimplify (current_node);
}
    return TREE_FUNC_STATUS_FAIL;
}

TreeFuncStatus MathTreeNodeNumAndNumSimplify (TreeNode *current_node) {

    assert (current_node);

    assert (current_node -> left_branch);
    assert (current_node -> right_branch);

    assert (current_node -> left_branch -> data);
    assert (current_node -> right_branch -> data);

    double value_after_simplify = NAN;

    value_after_simplify = MathTreeNodeBinaryCompute (BRANCH_VALUE (left_branch), BRANCH_VALUE (right_branch),
                                                      NODE_MATH_OPERATOR);

    TreeNode *temp_node = NUM_ (value_after_simplify);

    TreeNodeReplace (current_node, temp_node, sizeof (MathNode));

    free (temp_node);

    return TREE_FUNC_STATUS_OK;
}

TreeFuncStatus MathTreeNodeSmthAndZeroSimplify (TreeNode *current_node) {

    assert (current_node);

    assert (current_node -> left_branch);
    assert (current_node -> right_branch);

    assert (current_node -> left_branch -> data);
    assert (current_node -> right_branch -> data);

    TreeNode *branch_non_zero_ptr = NULL;
    TreeNextBranch branch_non_zero = NODE_NO_BRANCH;

    if (IS_VALUE_EQUAL (0, left_branch)) {

        branch_non_zero_ptr = current_node -> right_branch;
        branch_non_zero     = NODE_RIGHT_BRANCH;
    }

    if (IS_VALUE_EQUAL (0, right_branch)) {

        branch_non_zero_ptr = current_node -> left_branch;
        branch_non_zero     = NODE_LEFT_BRANCH;
    }

    if (!branch_non_zero_ptr || branch_non_zero == NODE_NO_BRANCH) {

        fprintf (stderr, "UNKNOWN ERROR IN BINARY ZERO SIMPLIFY (zero child nodes wasn't found)\n");
        return TREE_FUNC_STATUS_FAIL;
    }

    TreeNode *temp_node = NULL;

    switch (NODE_MATH_OPERATOR) {

        case OPERATOR_SUB:
            if (branch_non_zero == NODE_RIGHT_BRANCH)
                return TREE_FUNC_STATUS_FAIL;
        //fallthrough
        case OPERATOR_ADD:
            temp_node = TreeNodeCopy (temp_node, branch_non_zero_ptr, sizeof (MathNode));
            break;

        case OPERATOR_DIV:
            if (branch_non_zero == NODE_LEFT_BRANCH) {

                fprintf (stderr, "ERROR IN BINARY DIV SIMPLIFY (division by 0)\n");
                return TREE_FUNC_STATUS_FAIL;
            }
        //fallthrough
        case OPERATOR_POW:
            if (branch_non_zero == NODE_LEFT_BRANCH) {

                temp_node = NUM_ (1);
                break;
            }
        //fallthrough
        case OPERATOR_MUL:
            temp_node = NUM_ (0);
            break;


        default:
            fprintf (stderr, "UNKNOWN ERROR IN BINARY ZERO SIMPLIFY (node type wasn't found)\n");
            return TREE_FUNC_STATUS_FAIL;
    }

    TreeNodeReplace (current_node, temp_node, sizeof (MathNode));

    free (temp_node);

    return TREE_FUNC_STATUS_OK;
}

TreeFuncStatus MathTreeNodeSmthAndOneSimplify (TreeNode *current_node) {

    assert (current_node);

    assert (current_node -> left_branch);
    assert (current_node -> right_branch);

    assert (current_node -> left_branch -> data);
    assert (current_node -> right_branch -> data);

    TreeNode *branch_non_one_ptr = NULL;
    TreeNextBranch branch_non_one = NODE_NO_BRANCH;

    if (IS_VALUE_EQUAL (1, left_branch)) {

        branch_non_one_ptr = current_node -> right_branch;
        branch_non_one     = NODE_RIGHT_BRANCH;
    }

    if (IS_VALUE_EQUAL (1, right_branch)) {

        branch_non_one_ptr = current_node -> left_branch;
        branch_non_one     = NODE_LEFT_BRANCH;
    }

    TreeNode *temp_node = NULL;

    if (!branch_non_one_ptr || branch_non_one == NODE_NO_BRANCH) {

        fprintf (stderr, "UNKNOWN ERROR IN BINARY ONE SIMPLIFY (child nodes with one wasn't found)\n");
        return TREE_FUNC_STATUS_FAIL;
    }

    switch ((current_node -> data -> nodeValue).mathOperator) {

        case OPERATOR_MUL:
            temp_node = TreeNodeCopy (temp_node, branch_non_one_ptr, sizeof (MathNode));
            break;

        case OPERATOR_DIV:
            if (branch_non_one == NODE_RIGHT_BRANCH)
                return TREE_FUNC_STATUS_FAIL;

        //fallthrough
        case OPERATOR_POW:
            if (branch_non_one == NODE_LEFT_BRANCH)
                temp_node = TreeNodeCopy (temp_node, branch_non_one_ptr, sizeof (MathNode));

            else if (branch_non_one == NODE_RIGHT_BRANCH)
                temp_node = NUM_ (1);

            break;

        case OPERATOR_ADD:
        case OPERATOR_SUB:
            return TREE_FUNC_STATUS_FAIL;

        default:
            fprintf (stderr, "UNKNOWN ERROR OCCURED WHILST BINARY ONE SIMPLIFY\n");
            return TREE_FUNC_STATUS_FAIL;
    }

    TreeNodeReplace (current_node, temp_node, sizeof (MathNode));

    free (temp_node);

    return TREE_FUNC_STATUS_OK;
}

//govno below, have to be refactored ---------------------------------------------------------

TreeFuncStatus MathTreeNodeRead (FILE *file_for_read_tree, TreeNode **tree_node_for_fill) {  //PREORDER

    assert (file_for_read_tree);

    char *buf = (char *) calloc (NODE_READ_BUF_SIZE, sizeof (char));
    assert (buf);

    //HOW TO READ VARIABLE NUM OF SYMBOLS???

    if (IsBracketInFileStr (file_for_read_tree, '(') == false) {

        if (MathTreeNodeNilCheck (file_for_read_tree, buf) == TREE_FUNC_STATUS_OK)
            return TREE_FUNC_STATUS_OK;
        else
            return TREE_FUNC_STATUS_FAIL;
    }


    ON_TREE_DEBUG (printf ("( "));

    *tree_node_for_fill = CreateTreeNode ();

    if (MathTreeNodeDataRead (file_for_read_tree, tree_node_for_fill, buf) == TREE_FUNC_STATUS_FAIL)
        return TREE_FUNC_STATUS_FAIL;

    //recursion below

    if (MathTreeNodeRead (file_for_read_tree, &((*tree_node_for_fill) -> left_branch)) == TREE_FUNC_STATUS_FAIL)
        return TREE_FUNC_STATUS_FAIL;

    if (MathTreeNodeRead (file_for_read_tree, &((*tree_node_for_fill) -> right_branch)) == TREE_FUNC_STATUS_FAIL)
        return TREE_FUNC_STATUS_FAIL;

    //ON_TREE_DEBUG (printf ("|read two nodes|"));


    if (IsBracketInFileStr (file_for_read_tree, ')')) {

        ON_TREE_DEBUG (printf (") "));

        return TREE_FUNC_STATUS_OK;
    }

    return TREE_FUNC_STATUS_FAIL;
}

TreeFuncStatus MathTreeNodeNilCheck (FILE *file_for_node_nil_check, char *buffer_for_node_check) {

    assert (file_for_node_nil_check);
    assert (buffer_for_node_check);

    fscanf (file_for_node_nil_check, "%4s", buffer_for_node_check);

    if (strcmp (buffer_for_node_check, NIL) == 0) {

        ON_TREE_DEBUG (printf ("nil "));

        return TREE_FUNC_STATUS_OK;
    }

    ON_TREE_DEBUG (printf ("wtf, |%s|", buffer_for_node_check));

    return TREE_FUNC_STATUS_FAIL;
}

TreeFuncStatus MathTreeNodeDataRead (FILE *file_for_read_node_data, TreeNode **tree_node_for_data_read,
                                     char *buffer_for_read_node_data) {

    assert (file_for_read_node_data);
    assert (buffer_for_read_node_data);

    if (fscanf (file_for_read_node_data, " \" %100[^\"]", buffer_for_read_node_data)) {  //TODO fix num of read symbols

        if (strcmp ("add", buffer_for_read_node_data) == 0) {
            *tree_node_for_data_read = ADD_ (0, 0);
        }

        else if (strcmp ("sub", buffer_for_read_node_data) == 0) {
            *tree_node_for_data_read = SUB_ (0, 0);
        }

        else if (strcmp ("mul", buffer_for_read_node_data) == 0) {
            *tree_node_for_data_read = MUL_ (0, 0);
        }

        else if (strcmp ("div", buffer_for_read_node_data) == 0) {
            *tree_node_for_data_read = DIV_ (0, 0);
        }

        else if (strcmp ("pow", buffer_for_read_node_data) == 0) {
            *tree_node_for_data_read = POW_ (0, 0);
        }

        else if (strcmp ("sin", buffer_for_read_node_data) == 0) {
            *tree_node_for_data_read = SIN_ (0);
        }

        else if (strcmp ("cos", buffer_for_read_node_data) == 0) {
            *tree_node_for_data_read = COS_ (0);
        }

        else if (strcmp ("ln", buffer_for_read_node_data) == 0) {
            *tree_node_for_data_read = LN_ (0);
        }

        else if (strcmp ("x", buffer_for_read_node_data) == 0) {
            *tree_node_for_data_read = VAR_;
        }

        else
            *tree_node_for_data_read = NUM_ (strtod (buffer_for_read_node_data, 0));

        ON_TREE_DEBUG (printf("data "));

        fseek (file_for_read_node_data, 1, SEEK_CUR);

        return TREE_FUNC_STATUS_OK;
    }

    ON_TREE_DEBUG (printf ("wtf 1"));

    return TREE_FUNC_STATUS_FAIL;
}

#include <stdio.h>
#include <assert.h>

#include "../../lib/tree/tree_func.h"
#include "../../lib/tree/tree_log.h"

#include "../../lib/tree/math_tree/dsl_commands.h"
#include "../../lib/tree/math_tree/math_tree_func.h"

#include "backend.h"

//TODO add asserts to current_node -> data

/*
BackendFuncStatus TreeToAsmFile (FILE *asm_file, Tree *lang_tree, NameTable *name_table) {

    assert (asm_file);
    assert (lang_tree);
    assert (name_table);

    AsmFileBeginAndEnd (asm_file);

    AsmFileNewFuncWrite (asm_file, lang_tree -> root, name_table);

    return BACKEND_STATUS_OK;
}

BackendFuncStatus AsmFileBeginAndEnd (FILE *asm_file) {

    assert (asm_file);

    fprintf (asm_file, "call main\n"
                       "hlt\n");

    return BACKEND_STATUS_OK;
}

BackendFuncStatus AsmFileFuncNameWrite (FILE *asm_file, TreeNode *current_func_node, NameTable *name_table) {

    assert (current_func_node);
    assert (name_table);

    TreeNode *func_name_node = current_func_node -> right_branch -> left_branch;

    fprintf (asm_file, ":"); 
    NodeTypePrint (asm_file, func_name_node, name_table);

    return BACKEND_STATUS_OK;
}

BackendFuncStatus AsmFileNewFuncWrite (FILE *asm_file, TreeNode *current_node, NameTable *name_table) {

    assert (current_node);
    assert (name_table);

    if (NODE_TYPE == LANGUAGE_OPERATOR) {

        if (NODE_LANG_OPERATOR == NEW_FUNC) {

            AsmFileFuncNameWrite     (asm_file, current_node -> right_branch, name_table);
            AsmFileLangOperatorWrite (asm_file, 
                                      CURRENT_FUNC_FIRST_END_LINE_NODE (current_node -> right_branch),
                                      name_table);
        }

        else if (NODE_LANG_OPERATOR == INIT) {

            AsmFileFuncNameWrite     (asm_file, current_node -> left_branch, name_table);
            AsmFileLangOperatorWrite (asm_file, 
                                      CURRENT_FUNC_FIRST_END_LINE_NODE (current_node -> left_branch),
                                      name_table);

            return BACKEND_STATUS_OK;
        }
    }

    return AsmFileNewFuncWrite (asm_file, current_node -> left_branch, name_table);
}
*/
BackendFuncStatus AsmFileLangOperatorWrite (FILE *asm_file, TreeNode *current_node, NameTable *name_table) {

    assert (asm_file);
    assert (name_table);

    if (!current_node)
        return BACKEND_STATUS_OK;

    if (!(NODE_TYPE == LANGUAGE_OPERATOR && NODE_LANG_OPERATOR == END_LINE))
        return BACKEND_STATUS_FAIL;

    TreeNode *end_line_node = current_node;
   
    current_node = current_node -> left_branch;

    if (NODE_TYPE == LANGUAGE_OPERATOR) 
        switch (NODE_LANG_OPERATOR) {

            case IF:
                AsmFileOperatorIfWrite (asm_file, current_node, name_table);
                break;

            case WHILE:
                AsmFileOperatorWhileWrite (asm_file, current_node, name_table);
                break;

            case ASSIGN:
                AsmFileOperatorAssignWrite (asm_file, current_node, name_table);
                break;

            default:
                return BACKEND_STATUS_FAIL;
        }

    return AsmFileLangOperatorWrite (asm_file, end_line_node -> right_branch, name_table);    
}

BackendFuncStatus AsmFileOperatorIfWrite (FILE *asm_file, TreeNode *current_node, NameTable *name_table) {

    assert (asm_file);
    assert (current_node);
    assert (name_table);

    static size_t operator_if_number = 0;

    AsmFileConditionWrite (asm_file, current_node -> left_branch, name_table);

    fprintf (asm_file, "push 0\n"
                       "je end_if_%zu\n", operator_if_number);

    AsmFileLangOperatorWrite (asm_file, current_node -> right_branch, name_table);

    fprintf (asm_file, ":end_if_%zu\n", operator_if_number);

    operator_if_number++;

    return BACKEND_STATUS_OK;
}

BackendFuncStatus AsmFileOperatorWhileWrite (FILE *asm_file, TreeNode *current_node, NameTable *name_table) {

    assert (asm_file);
    assert (current_node);
    assert (name_table);

    static size_t operator_while_number = 0;

    fprintf (asm_file, ":while_%zu\n", operator_while_number);

    AsmFileConditionWrite (asm_file, current_node -> left_branch, name_table);

    fprintf (asm_file, "push 0\n"
                       "je end_while_%zu\n", operator_while_number);

    AsmFileLangOperatorWrite (asm_file, current_node -> right_branch, name_table);

    fprintf (asm_file, "jmp while_%zu\n"
                       ":end_while_%zu\n",
                       operator_while_number, operator_while_number);

    operator_while_number++;

    return BACKEND_STATUS_OK;
}

BackendFuncStatus AsmFileOperatorOrAndWrite (FILE *asm_file, TreeNode *current_node, NameTable *name_table) {

    assert (asm_file);
    assert (current_node);
    assert (name_table);

    if (NODE_TYPE == LANGUAGE_OPERATOR && (NODE_LANG_OPERATOR == OR || NODE_LANG_OPERATOR == AND)) {
        
        AsmFileOperatorOrAndWrite      (asm_file, current_node -> left_branch, name_table);

        AsmFileOperatorComparisonWrite (asm_file, current_node -> right_branch, name_table);
    }

    else 
        return AsmFileOperatorComparisonWrite (asm_file, current_node, name_table);
    
    if (NODE_TYPE == LANGUAGE_OPERATOR)
        switch (NODE_LANG_OPERATOR) {

            case OR:
                fprintf (asm_file, "add\n");
                break;

            case AND:
                fprintf (asm_file, "mul\n");
                break;
            
            default:
                return BACKEND_STATUS_FAIL;
        }

    return BACKEND_STATUS_OK;
}

BackendFuncStatus AsmFileOperatorComparisonWrite (FILE *asm_file, TreeNode *current_node,
                                                  NameTable *name_table) {

    assert (asm_file);
    assert (current_node);

    AsmFileMathExpressionWrite (asm_file, current_node -> left_branch, name_table);
    AsmFileMathExpressionWrite (asm_file, current_node -> right_branch, name_table);

    if (NODE_TYPE == BINARY_OPERATOR)
        switch (NODE_MATH_OPERATOR) {

            case OPERATOR_GREATER:
                fprintf (asm_file, "ja ");
                break;
        
            case OPERATOR_LESS:
                fprintf (asm_file, "jb ");
                break;
          
            case OPERATOR_EQUAL:
                fprintf (asm_file, "je ");
                break;

            case OPERATOR_NOT_EQUAL:
                fprintf (asm_file, "jne ");
                break;

            default:
                return BACKEND_STATUS_FAIL;
        }

    static size_t comparison_num = 0;

    fprintf (asm_file, "comparison_%zu\n"
                       "push 0\n"
                       "jmp comparison_end_%zu\n"
                       ":comparison_%zu\n"
                       "push 1\n"
                       ":comparison_end_%zu\n",
                        comparison_num, comparison_num, comparison_num, comparison_num);

    comparison_num++;

    return BACKEND_STATUS_OK;
}


BackendFuncStatus AsmFileConditionWrite (FILE *asm_file, TreeNode *current_node, NameTable *name_table) {

    assert (asm_file);
    assert (current_node);
    assert (name_table);

    return AsmFileOperatorOrAndWrite (asm_file, current_node, name_table);
}

BackendFuncStatus AsmFileOperatorAssignWrite (FILE *asm_file, TreeNode *current_node, NameTable *name_table) {

    assert (asm_file);
    assert (current_node);
    assert (name_table);

    AsmFileMathExpressionWrite (asm_file, current_node -> right_branch, name_table);

    current_node = current_node -> left_branch;
    fprintf (asm_file, "pop [%zu]\n", size_t (NODE_VALUE));

    return BACKEND_STATUS_OK;
}

BackendFuncStatus AsmFileMathExpressionWrite (FILE *asm_file, TreeNode *current_node, NameTable *name_table) {

    assert (asm_file);
    assert (current_node);
    assert (name_table);  

    if (NODE_TYPE == NUMBER || NODE_TYPE == VARIABLE) {

        AsmFileVarOrNumWrite (asm_file, current_node, name_table);
        return BACKEND_STATUS_OK;
    }

    if (AsmFileMathExpressionWrite (asm_file, current_node -> left_branch, name_table) == BACKEND_STATUS_FAIL)
        return BACKEND_STATUS_FAIL;
    
    if (AsmFileMathExpressionWrite (asm_file, current_node -> right_branch, name_table) == BACKEND_STATUS_FAIL)
        return BACKEND_STATUS_FAIL;

    if (AsmFileMathOperatorWrite (asm_file, current_node) == BACKEND_STATUS_FAIL)
        return BACKEND_STATUS_FAIL;

    return BACKEND_STATUS_OK;
}

BackendFuncStatus AsmFileVarOrNumWrite (FILE *asm_file, TreeNode *current_node, NameTable *name_table) {

    assert (asm_file);
    assert (current_node);
    assert (name_table);

    if (NODE_TYPE == NUMBER)
        fprintf (asm_file, "push %lf\n", NODE_VALUE);
    
    else if (NODE_TYPE == VARIABLE)
        fprintf (asm_file, "push [%zu]\n", size_t (NODE_VALUE)); 

    else 
        return BACKEND_STATUS_FAIL;

    return BACKEND_STATUS_OK;
}

BackendFuncStatus AsmFileMathOperatorWrite (FILE *asm_file, TreeNode *current_node) {

    assert (asm_file);
    assert (current_node);

    if (NODE_TYPE == BINARY_OPERATOR) //TODO as function
        switch (NODE_MATH_OPERATOR) {

            case OPERATOR_ADD:
                fprintf (asm_file, "add\n");
                break;

            case OPERATOR_SUB:
                fprintf (asm_file, "sub\n");
                break;

            case OPERATOR_MUL:
                fprintf (asm_file, "mul\n");
                break;

            case OPERATOR_DIV:
                fprintf (asm_file, "div\n");
                break;

            default:
                return BACKEND_STATUS_FAIL;
        }

    return BACKEND_STATUS_OK;
}

#include <stdio.h>
#include <stdlib.h>

#include "../../lib/tree/tree_func.h"
#include "../../lib/tree/tree_log.h"

#include "../../lib/tree/math_tree/dsl_commands.h"
#include "../../lib/tree/math_tree/math_tree_func.h"

#include "semantic_analyzer.h"

unsigned int SemanticAnalyzer (const Tree *lang_tree, const NameTable *global_name_table) {

    NAME_TABLE_VERIFY_UNSIGNED_FUNC (global_name_table);

    MATH_TREE_VERIFY_UNSIGNED_FUNC (lang_tree, global_name_table);

    const TreeNode *lang_tree_root = lang_tree -> root;

    NameTable all_func_name_table = {};
    NameTableCtor (&all_func_name_table);

    if (AllFuncNameTableFill (lang_tree_root, &all_func_name_table, global_name_table) ==
                                                                            SEMANTIC_FUNC_STATUS_FAIL) {

        NameTableDtor (&all_func_name_table);
        return DEAD_NAME_TABLE;
    }

    unsigned int semantic_errors = MultipleFuncDefinitionCheck (&all_func_name_table)          |
                                   VarsDefinitionCheck (lang_tree_root, &all_func_name_table,
                                                        global_name_table);

    NameTableDtor (&all_func_name_table);

    return semantic_errors;
}

SemanticFuncStatus AllFuncNameTableFill (const TreeNode *current_node, NameTable *all_func_name_table,
                                         const NameTable *global_name_table) {

    MATH_TREE_NODE_VERIFY (current_node, SEMANTIC);

    NAME_TABLE_VERIFY (global_name_table, SEMANTIC);

    NAME_TABLE_VERIFY (all_func_name_table, SEMANTIC);

    while (current_node && NODE_TYPE == LANGUAGE_OPERATOR) {

        const TreeNode *new_func_node = current_node;

        switch (NODE_LANG_OPERATOR) {

            case NEW_FUNC:
                current_node = current_node -> right_branch;
                break;

            case INIT:
                break;

            default:
                return SEMANTIC_FUNC_STATUS_FAIL;
        }

        current_node = CURRENT_FUNC_NAME_NODE (current_node);

        const char *func_name               = NameTableVariableFind ((size_t) NODE_VALUE, global_name_table);
        const unsigned int func_args_amount = FuncArgsCount (current_node);

        NameTableAdd (all_func_name_table, NAME_TABLE_VARIABLE, func_name, func_args_amount);

        current_node = new_func_node;

        switch (NODE_LANG_OPERATOR) {

            case NEW_FUNC:
                if (!(current_node = new_func_node -> left_branch))
                    return SEMANTIC_FUNC_STATUS_OK;

                break;

            case INIT:
                return SEMANTIC_FUNC_STATUS_OK;

            default:
                return SEMANTIC_FUNC_STATUS_FAIL;
        }
    }

    return SEMANTIC_FUNC_STATUS_FAIL;
}

unsigned int FuncArgsCount (const TreeNode *current_node) {

    MATH_TREE_NODE_VERIFY_UNSIGNED_FUNC (current_node);

    current_node = CURRENT_FUNC_NAME_NODE (current_node) -> left_branch;

    unsigned int args_amount = 0;

    if (!current_node)
        return args_amount;

    args_amount++;

    while (NODE_TYPE == LANGUAGE_OPERATOR && NODE_LANG_OPERATOR == COMMA) {

        args_amount++;
        current_node = current_node -> right_branch;
    }

     return args_amount;
}

unsigned int MultipleFuncDefinitionCheck (const NameTable *all_func_name_table) {

    NAME_TABLE_VERIFY_UNSIGNED_FUNC (all_func_name_table);

    const char *multiple_func_name = NameTableRepeatCheck (all_func_name_table);

    if (multiple_func_name) {

        fprintf (stderr, "MULTIPLE DEFINITION of FUNC '%s()'\n", multiple_func_name);
        return (unsigned int) MULTIPLE_FUNC_DEFINITION;
    }

    return (unsigned int) NO_ERRORS;
}

unsigned int VarsDefinitionCheck (const TreeNode *current_node, const NameTable *all_func_name_table,
                                  const NameTable *global_name_table) {

    MATH_TREE_NODE_VERIFY_UNSIGNED_FUNC (current_node);

    NAME_TABLE_VERIFY_UNSIGNED_FUNC (all_func_name_table);
    NAME_TABLE_VERIFY_UNSIGNED_FUNC (global_name_table);

    while (NODE_TYPE == LANGUAGE_OPERATOR) {

        const TreeNode *new_func_node = current_node;

        switch (NODE_LANG_OPERATOR) {

            case NEW_FUNC:
                current_node = current_node -> right_branch;
                break;

            case INIT:
                break;

            default:
                fprintf (stderr, "UNKNOWN ERROR HAPPENED during VARIABLES CHECK");
                return DEAD_TREE;
        }

        unsigned int var_errors = VarsInFuncCheck (current_node, all_func_name_table, global_name_table);

        if (var_errors != 0)
            return var_errors;

        current_node = new_func_node;

        switch (NODE_LANG_OPERATOR) {

            case NEW_FUNC:
                if (!(current_node = new_func_node -> left_branch))
                    return (unsigned int) NO_ERRORS;

                break;

            case INIT:
                return (unsigned int) NO_ERRORS;

            default:
                return DEAD_TREE_NODE;
        }
    }

    return (unsigned int) NO_ERRORS;
}

unsigned int VarsInFuncCheck (const TreeNode *func_node, const NameTable *all_func_name_table,
                              const NameTable *global_name_table) {

    MATH_TREE_NODE_VERIFY_UNSIGNED_FUNC (func_node);

    NAME_TABLE_VERIFY_UNSIGNED_FUNC (all_func_name_table);
    NAME_TABLE_VERIFY_UNSIGNED_FUNC (global_name_table);

    NameTable func_local_name_table = {};
    NameTableCtor (&func_local_name_table);

    if (FuncArgsNameTableFill (func_node, global_name_table, &func_local_name_table) ==
                                                                                SEMANTIC_FUNC_STATUS_FAIL)

        return DEAD_NAME_TABLE;

    TreeNode *end_line_node = CURRENT_FUNC_FIRST_END_LINE_NODE (func_node);

    VarNameTableRecursiveFill (end_line_node, global_name_table, &func_local_name_table);

    unsigned int var_errors = MultipleVarsDefinitionCheck (&func_local_name_table)            |
                              UndeclaredVarsCheck (end_line_node, all_func_name_table,
                                                   global_name_table, &func_local_name_table) |
                              VarAsFuncUsing (end_line_node, all_func_name_table, &func_local_name_table);
    if (var_errors == UNDECLARED_VAR)
        fprintf (stderr, "varerror");

    NameTableDtor (&func_local_name_table);

    return var_errors;
}

unsigned int VarAsFuncUsing (const TreeNode *current_node, const NameTable *all_func_name_table,
                             const NameTable *func_local_name_table) {

    if (!current_node)
        return (unsigned int) NO_ERRORS;

    MATH_TREE_NODE_VERIFY_UNSIGNED_FUNC (current_node);

    NAME_TABLE_VERIFY_UNSIGNED_FUNC (all_func_name_table);
    NAME_TABLE_VERIFY_UNSIGNED_FUNC (func_local_name_table);

    unsigned int var_as_func_error = (unsigned int) NO_ERRORS;

    if (NODE_TYPE == LANGUAGE_OPERATOR && NODE_LANG_OPERATOR == FUNC_CALL) {

        current_node = current_node -> left_branch;

        const char *called_func_name = NameTableVariableFind ((size_t) NODE_VALUE, func_local_name_table);

        if (NameTableWordFind (all_func_name_table, called_func_name, 0) == -1)
            var_as_func_error = (unsigned int) VAR_AS_FUNC_USAGE;

        return var_as_func_error;
    }

    var_as_func_error |= VarAsFuncUsing (current_node -> left_branch, all_func_name_table,
                                         func_local_name_table);
    var_as_func_error |= VarAsFuncUsing (current_node -> right_branch, all_func_name_table,
                                         func_local_name_table);

    return var_as_func_error;
}

unsigned int UndeclaredVarsCheck (const TreeNode *current_node, const NameTable *all_func_name_table,
                                  const NameTable *global_name_table, const NameTable *func_local_name_table) {

    if (!current_node)
        return (unsigned int) NO_ERRORS;

    MATH_TREE_NODE_VERIFY_UNSIGNED_FUNC (current_node);

    NAME_TABLE_VERIFY_UNSIGNED_FUNC (all_func_name_table);
    NAME_TABLE_VERIFY_UNSIGNED_FUNC (global_name_table);
    NAME_TABLE_VERIFY_UNSIGNED_FUNC (func_local_name_table);

    unsigned int undeclared_vars_error = (unsigned int) NO_ERRORS;

    if (NODE_TYPE == VARIABLE) {

        const char *current_var_name = NameTableVariableFind ((size_t) NODE_VALUE, global_name_table);

        if (NameTableWordFind (all_func_name_table, current_var_name, 0) == -1)
            if (NameTableWordFind (func_local_name_table, current_var_name, 0) == -1) {

                fprintf (stderr, "UNDEFINED VARIABLE '%s'\n", current_var_name);
                undeclared_vars_error = (unsigned int) UNDECLARED_VAR;
            }

        return undeclared_vars_error;
    }

    if (NODE_TYPE == LANGUAGE_OPERATOR)
        switch (NODE_LANG_OPERATOR) {

            case INIT:
                undeclared_vars_error |= UndeclaredVarsCheck (current_node -> right_branch -> right_branch,
                                                              all_func_name_table, global_name_table,
                                                              func_local_name_table);
                break;

            default:
                undeclared_vars_error |= UndeclaredVarsCheck (current_node -> left_branch,
                                                              all_func_name_table, global_name_table,
                                                              func_local_name_table);

                undeclared_vars_error |= UndeclaredVarsCheck (current_node -> right_branch,
                                                              all_func_name_table, global_name_table,
                                                              func_local_name_table);
        }


    return undeclared_vars_error;
}

unsigned int MultipleVarsDefinitionCheck (const NameTable *func_local_name_table) {

    NAME_TABLE_VERIFY_UNSIGNED_FUNC (func_local_name_table);

    const char *multiple_var_name = NameTableRepeatCheck (func_local_name_table);

    if (multiple_var_name) {

        fprintf (stderr, "MULTIPLE DEFINITION of VARIABLE '%s'\n", multiple_var_name);
        return (unsigned int) MULTIPLE_VAR_DEFINITION;
    }

    return (unsigned int) NO_ERRORS;
}

SemanticFuncStatus FuncArgsNameTableFill (const TreeNode *func_node, const NameTable *global_name_table,
                                          NameTable *func_local_name_table) {

    MATH_TREE_NODE_VERIFY (func_node, SEMANTIC);

    NAME_TABLE_VERIFY (global_name_table, SEMANTIC);
    NAME_TABLE_VERIFY (func_local_name_table, SEMANTIC);

    TreeNode *func_args_node = CURRENT_FUNC_NAME_NODE (func_node) -> left_branch;

    if (!func_args_node)
        return SEMANTIC_FUNC_STATUS_OK;

    TreeNode *current_node = func_args_node;

    while (NODE_TYPE == LANGUAGE_OPERATOR) {

        switch (NODE_LANG_OPERATOR) {

            case COMMA:
                current_node = func_args_node -> left_branch -> right_branch;
                break;

            case INIT:
                current_node = func_args_node -> right_branch;
                break;

            default:
                return SEMANTIC_FUNC_STATUS_FAIL;
        }

        const char *current_arg_name = NameTableVariableFind ((size_t) NODE_VALUE, global_name_table);

        NameTableAdd (func_local_name_table, NAME_TABLE_VARIABLE, current_arg_name, 0);

        switch (NODE_LANG_OPERATOR) {

            case COMMA:
                func_args_node = func_args_node -> right_branch;
                current_node = func_args_node;
                break;

            case INIT:
                return SEMANTIC_FUNC_STATUS_OK;

            default:
                return SEMANTIC_FUNC_STATUS_FAIL;
        }
    }

    return SEMANTIC_FUNC_STATUS_FAIL;
}

SemanticFuncStatus VarNameTableRecursiveFill (const TreeNode *end_line_node,
                                              const NameTable *global_name_table,
                                              NameTable *func_local_name_table) {

    MATH_TREE_NODE_VERIFY (end_line_node, SEMANTIC);

    NAME_TABLE_VERIFY (global_name_table, SEMANTIC);
    NAME_TABLE_VERIFY (func_local_name_table, SEMANTIC);

    const TreeNode *current_node = end_line_node;

    while (end_line_node && NODE_TYPE == LANGUAGE_OPERATOR && NODE_LANG_OPERATOR == END_LINE) {

        current_node = end_line_node -> left_branch;

        if (NODE_TYPE == LANGUAGE_OPERATOR)
            switch (NODE_LANG_OPERATOR) {

                case IF:
                case WHILE:
                    VarNameTableRecursiveFill (current_node -> right_branch, global_name_table,
                                               func_local_name_table);
                    break;

                case INIT: {

                    current_node         = current_node -> right_branch -> left_branch;
                    const char *var_name = NameTableVariableFind ((size_t) NODE_VALUE, global_name_table);

                    NameTableAdd (func_local_name_table, NAME_TABLE_VARIABLE, var_name, 0);
                }
                //fallthrough
                default:
                    return SEMANTIC_FUNC_STATUS_OK;
            }

        end_line_node = end_line_node -> right_branch;
    }

    return SEMANTIC_FUNC_STATUS_OK;
}

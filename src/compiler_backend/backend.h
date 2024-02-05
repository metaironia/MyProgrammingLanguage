#ifndef BACKEND_H
#define BACKEND_H

#include "../../lib/tree/tree_func.h"
#include "../../lib/tree/tree_log.h"

#include "../../lib/tree/math_tree/dsl_commands.h"
#include "../../lib/tree/math_tree/math_tree_func.h"

#define CURRENT_FUNC_FIRST_END_LINE_NODE(current_node)                                                      \
                                                current_node -> right_branch -> left_branch -> right_branch

enum BackendFuncStatus {

    BACKEND_STATUS_FAIL,
    BACKEND_STATUS_OK
};

BackendFuncStatus TreeToAsmFile (FILE *asm_file, Tree *lang_tree, NameTable *name_table);

BackendFuncStatus AsmFileBeginAndEnd (FILE *asm_file);

BackendFuncStatus AsmFileFuncNameWrite (FILE *asm_file, TreeNode *current_func_node, NameTable *name_table);

BackendFuncStatus AsmFileNewFuncWrite (FILE *asm_file, TreeNode *current_node, NameTable *name_table);

BackendFuncStatus AsmFileLangOperatorWrite (FILE *asm_file, TreeNode *current_node, NameTable *name_table);

BackendFuncStatus AsmFileOperatorIfWrite (FILE *asm_file, TreeNode *current_node, NameTable *name_table);

BackendFuncStatus AsmFileOperatorWhileWrite (FILE *asm_file, TreeNode *current_node, NameTable *name_table);

BackendFuncStatus AsmFileOperatorOrAndWrite (FILE *asm_file, TreeNode *current_node, NameTable *name_table);

BackendFuncStatus AsmFileOperatorComparisonWrite (FILE *asm_file, TreeNode *current_node,
                                                  NameTable *name_table);

BackendFuncStatus AsmFileConditionWrite (FILE *asm_file, TreeNode *current_node, NameTable *name_table);                                                

BackendFuncStatus AsmFileOperatorAssignWrite (FILE *asm_file, TreeNode *current_node, NameTable *name_table);

BackendFuncStatus AsmFileMathExpressionWrite (FILE *asm_file, TreeNode *current_node, NameTable *name_table);

BackendFuncStatus AsmFileVarOrNumWrite (FILE *asm_file, TreeNode *current_node, NameTable *name_table);

BackendFuncStatus AsmFileMathOperatorWrite (FILE *asm_file, TreeNode *current_node);

#endif
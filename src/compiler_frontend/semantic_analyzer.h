#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#define CURRENT_FUNC_NAME_NODE(current_node)            current_node -> right_branch -> left_branch

#define CURRENT_FUNC_FIRST_END_LINE_NODE(current_node)  CURRENT_FUNC_NAME_NODE (current_node) -> right_branch

enum SemanticFuncStatus {

    SEMANTIC_FUNC_STATUS_OK,
    SEMANTIC_FUNC_STATUS_FAIL
};

enum SemanticErrors {

    NO_ERRORS = 0,
    MULTIPLE_FUNC_DEFINITION = (1 << 0),
    MULTIPLE_VAR_DEFINITION = (1 << 1),
    UNDECLARED_VAR = (1 << 2),
    FUNC_WRONG_NUM_PASSED_ARGS = (1 << 3),
    VAR_AS_FUNC_USAGE = (1 << 4)
};

unsigned int SemanticAnalyzer (const Tree *lang_tree, const NameTable *global_name_table);

SemanticFuncStatus AllFuncNameTableFill (const TreeNode *current_node, NameTable *all_func_name_table,
                                         const NameTable *global_name_table);

unsigned int FuncArgsCount (const TreeNode *current_node);

unsigned int MultipleFuncDefinitionCheck (const NameTable *all_func_name_table);

unsigned int VarsDefinitionCheck (const TreeNode *current_node, const NameTable *all_func_name_table,
                                  const NameTable *global_name_table);

unsigned int VarsInFuncCheck (const TreeNode *func_node, const NameTable *all_func_name_table,
                              const NameTable *global_name_table);

unsigned int VarAsFuncUsing (const TreeNode *current_node, const NameTable *all_func_name_table,
                             const NameTable *global_name_table);

unsigned int UndeclaredVarsCheck (const TreeNode *current_node, const NameTable *all_func_name_table,
                                  const NameTable *global_name_table, const NameTable *func_local_name_table);

unsigned int MultipleVarsDefinitionCheck (const NameTable *func_local_name_table);

SemanticFuncStatus FuncArgsNameTableFill (const TreeNode *func_node, const NameTable *global_name_table,
                                          NameTable *func_local_name_table);

SemanticFuncStatus VarNameTableRecursiveFill (const TreeNode *end_line_node,
                                              const NameTable *global_name_table,
                                              NameTable *func_local_name_table);

#endif

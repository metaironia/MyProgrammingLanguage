#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

#include <stdio.h>

#include "../../lib/tree/tree_func.h"
#include "../../lib/tree/math_tree/math_tree_func.h"

#define CHECK_WORD_LANGUAGE_OP(current_word, keyword)                                                       \
                                {                                                                           \
                                    for (size_t i = 0;                                                      \
                                        i < sizeof (keyword##_QUOTES) / sizeof (keyword##_QUOTES)[0];       \
                                        i++)                                                                \
                                                                                                            \
                                        if (strstr (current_word, keyword##_QUOTES[i])) {                   \
                                                                                                            \
                                            current_node = CreateLangTreeNode (keyword, NULL,               \
                                                                                        NULL);              \
                                            current_node++;                                                 \
                                                                                                            \
                                            break;                                                          \
                                        };                                                                  \
                                                                                                            \
                                        break;                                                              \
                                }                                                                   

#define CHECK_WORD_MATH_OP(current_word, keyword_type, keyword)                                                        \
                                {                                                                                      \
                                    for (size_t i = 0;                                                                 \
                                         i < sizeof (keyword##_QUOTES) / sizeof (keyword##_QUOTES)[0];                 \
                                         i++)                                                                          \
                                                                                                                       \
                                                                                                                       \
                                        if (strstr (current_word, keyword##_QUOTES[i])) {                              \
                                                                                                                       \
                                            current_node = CreateMathTreeNode(keyword_type, OPERATOR_##keyword, NULL,  \
                                                                                                                NULL); \
                                            current_node++;                                                            \
                                                                                                                       \
                                            break;                                                                     \
                                        };                                                                             \
                                                                                                                       \
                                        break;                                                                         \
                                }

enum LexicalFuncStatus {

    LEXICAL_FUNC_STATUS_OK,
    LEXICAL_FUNC_STATUS_FAIL
};

const int MAX_WORD_LENGTH    = 64;
const int MAX_PROGRAM_LENGHT = 10000; 

struct LanguageToken {

    char **char_array;
    TreeNode *node_array;
};

LexicalFuncStatus LexicalAnalyzer (FILE *input_file, LanguageToken *token_struct);

LexicalFuncStatus StringInputFromFile (FILE *input_file, char **input_array);

LexicalFuncStatus StringTokenSeparate (LanguageToken *token_struct);

TreeNode *CreateLangTreeNode (const LangNodeOperator node_operator, TreeNode *const ptr_left_branch,
                              TreeNode *const ptr_right_branch);

bool LexemeCheckIfNumber (char *word_to_check, TreeNode **current_node);

bool LexemeCheckIfVariable (const char *word_to_check, TreeNode **current_node);

const char *LangNodeTypeToString (const TreeNode *current_node);

const char *LangNodeOperatorToString (const LangNodeOperator current_operator);

#endif
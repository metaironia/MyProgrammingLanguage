#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

#include <stdio.h>

#include "../../lib/tree/tree_func.h"
#include "../../lib/tree/math_tree/math_tree_func.h"

#define CHECK_WORD_LANGUAGE_OP(current_word, keyword)                                                 \
            {                                                                                         \
                for (size_t i = 0; i < sizeof (keyword##_QUOTES) / sizeof (keyword##_QUOTES)[0]; i++) \
                                                                                                      \
                    if (strstr (current_word, keyword##_QUOTES[i])) {                                 \
                                                                                                      \
                        if ((strcmp (keyword##_QUOTES[i], "но") == 0 ||                               \
                            (strcmp (keyword##_QUOTES[i], "не") == 0) ||                              \                               
                            (strcmp (keyword##_QUOTES[i], "и") == 0)) &&                              \
                            strcmp (keyword##_QUOTES[i], current_word) != 0)                          \
                                break;                                                                \
                                                                                                      \
                        (token_struct -> index_node_word)[node_array_index] = char_array_index;       \
                        *current_node++ = CreateLangTreeNode (keyword, NULL, NULL);                   \
                        is_success      = true;                                                       \
                        break;                                                                        \
                    }                                                                                 \
                                                                                                      \
                if (is_success)                                                                       \
                    continue;                                                                         \
            }                                                                   

#define CHECK_WORD_MATH_OP(current_word, keyword_type, keyword)                                               \
            {                                                                                                 \
                for (size_t i = 0; i < sizeof (keyword##_QUOTES) / sizeof (keyword##_QUOTES)[0]; i++)         \
                                                                                                              \
                    if (strstr (current_word, keyword##_QUOTES[i])) {                                         \
                                                                                                              \
                        (token_struct -> index_node_word)[node_array_index] = char_array_index;               \
                        *current_node++ = CreateMathTreeNode (keyword_type, OPERATOR_##keyword, NULL, NULL);  \
                        is_success      = true;                                                               \
                        break;                                                                                \
                    }                                                                                         \
                                                                                                              \
                if (is_success)                                                                               \
                    continue;                                                                                 \
            }

enum LexicalFuncStatus {

    LEXICAL_FUNC_STATUS_OK,
    LEXICAL_FUNC_STATUS_FAIL
};

const int MAX_WORD_LENGTH    = 64;
const int MAX_PROGRAM_LENGTH = 10000; 

struct LanguageToken {

    char **char_array;
    TreeNode **node_array;
    size_t *index_node_word;
};

LexicalFuncStatus LexicalAnalyzer (FILE *input_file, LanguageToken *token_struct, NameTable *name_table);

LexicalFuncStatus StringInputFromFile (FILE *input_file, char **input_array);

LexicalFuncStatus StringTokenSeparate (LanguageToken *token_struct, NameTable *name_table);

#endif
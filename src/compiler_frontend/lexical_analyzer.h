#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

#include <stdio.h>

#include "../../lib/tree/tree_func.h"
#include "../../lib/tree/math_tree/math_tree_func.h"

#define LANG_TOKEN_VERIFY(lang_token, parent_func)  if (LangTokenVerify (lang_token) != 0) {    \
                                                                                                \
                                                        LangTokenDump (lang_token);             \
                                                        return parent_func##_FUNC_STATUS_FAIL;  \
                                                    }                                           \

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
                        ((token_struct -> data).index_node_word)[node_arr_index] = char_arr_index;    \
                        *current_node++ = CreateLangTreeNode (keyword, NULL, NULL);                   \
                        (token_struct -> node_size)++;                                                \
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
                        ((token_struct -> data).index_node_word)[node_arr_index] = char_arr_index;            \
                        *current_node++ = CreateMathTreeNode (keyword_type, OPERATOR_##keyword, NULL, NULL);  \
                        (token_struct -> node_size)++;                                                        \
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

const int MAX_WORD_LENGTH    = 64;    // ?
const int MAX_PROGRAM_LENGTH = 10000; // to delete

const int DEFAULT_DATA_CAPACITY = 2;
const int LANG_TOKEN_POISON     = 0xDEAD;
const int INCREASE_NUM          = 2;

struct LanguageTokenData {

    char     **char_array;        // not const because of strstr()
    TreeNode **node_array;
    size_t    *index_node_word;
};

struct LanguageToken {

    LanguageTokenData  data;
    size_t             char_size;
    size_t             node_size;
    size_t             data_capacity;
};

enum LangTokenErrors {

    LANG_TOKEN_NULL_PTR = (1 << 0),
    LANG_TOKEN_CHAR_NULL_PTR = (1 << 1),
    LANG_TOKEN_NODE_NULL_PTR = (1 << 2),
    LANG_TOKEN_INDEX_NULL_PTR = (1 << 3),
    LANG_TOKEN_CHAR_LESS_THAN_NODE = (1 << 4)
};

LexicalFuncStatus LangTokenCtor (LanguageToken *token_struct);

LexicalFuncStatus LangTokenDtor (LanguageToken *token_struct);

LexicalFuncStatus LangTokenDataCtor (LanguageToken *token_struct);

LexicalFuncStatus LangTokenDataDtor (LanguageToken *token_struct);

LexicalFuncStatus LangTokenNodeAndIndexAdd (LanguageToken *token_struct, TreeNode *token_node, 
                                            const size_t token_index);

LexicalFuncStatus LangTokenWordAdd (LanguageToken *token_struct, char *token_word);

LexicalFuncStatus LangTokenRecalloc (LanguageToken *token_struct);            

LexicalFuncStatus LangTokenDump (const LanguageToken *token_struct);

unsigned int LangTokenVerify (const LanguageToken *token_struct);

LexicalFuncStatus LexicalAnalyzer (FILE *input_file, LanguageToken *token_struct, NameTable *name_table);

LexicalFuncStatus StringInputFromFile (FILE *input_file, LanguageToken *token_struct);

LexicalFuncStatus StringTokenSeparate (LanguageToken *token_struct, NameTable *name_table);

#endif

#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

#include <stdio.h>

#include "../../lib/tree/tree_func.h"
#include "../../lib/tree/math_tree/math_tree_func.h"

#define TOKEN_CHAR_ARR                              ((token_struct -> data).char_array)
#define TOKEN_NODE_ARR                              ((token_struct -> data).node_array)
#define TOKEN_INDEX_NODE                            ((token_struct -> data).index_node_word)

#define LANG_TOKEN_VERIFY(lang_token, parent_func)  if (LangTokenVerify (lang_token) != 0) {    \
                                                                                                \
                                                        LangTokenDump (lang_token);             \
                                                        return parent_func##_FUNC_STATUS_FAIL;  \
                                                    }                                           \

#define LANG_TOKEN_VERIFY_PTR_FUNC(lang_token)      if (LangTokenVerify (lang_token) != 0) {    \
                                                                                                \
                                                        LangTokenDump (lang_token);             \
                                                        return NULL;                            \
                                                    }

#define CHECK_WORD_LANGUAGE_OP(current_word, keyword)                                            \
            {                                                                                    \
                is_success = WordCheckLanguageOperator (keyword##_QUOTES,                        \
                                                        sizeof (keyword##_QUOTES) / sizeof (keyword##_QUOTES)[0], \
                                                        current_word);                           \
                                                                                                 \
                if (is_success) {                                                                \
                                                                                                 \
                    TOKEN_INDEX_NODE[node_arr_index] = char_arr_index;                           \
                    *current_node++                  = CreateLangTreeNode (keyword, NULL, NULL); \
                    (token_struct -> node_size)++;                                               \
                                                                                                 \
                    continue;                                                                    \
                }                                                                                \
            }

#define CHECK_WORD_MATH_OP(current_word, keyword_type, keyword)                                                \
            {                                                                                                  \
                is_success = WordCheckMathOperator (keyword##_QUOTES,                                          \
                                                    sizeof (keyword##_QUOTES) / sizeof (keyword##_QUOTES)[0],  \
                                                    current_word);                                             \
                                                                                                               \
                if (is_success) {                                                                              \
                                                                                                               \
                    TOKEN_INDEX_NODE[node_arr_index] = char_arr_index;                                         \
                    *current_node++                  = CreateMathTreeNode (keyword_type, OPERATOR_##keyword,   \
                                                                           NULL, NULL);                        \
                    (token_struct -> node_size)++;                                                             \
                                                                                                               \
                    continue;                                                                                  \
                }                                                                                              \
            }

enum LexicalFuncStatus {

    LEXICAL_FUNC_STATUS_OK,
    LEXICAL_FUNC_STATUS_FAIL
};

const int MAX_WORD_LENGTH    = 64;    // ?

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

bool WordCheckLanguageOperator (const char *const *quotes_arr, const size_t quotes_arr_size, const char *word);

bool WordCheckMathOperator (const char *const *quotes_arr, const size_t quotes_arr_size, const char *word);

#endif

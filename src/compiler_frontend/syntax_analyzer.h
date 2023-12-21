#ifndef SYNTAX_ANALYZER_H
#define SYNTAX_ANALYZER_H

#include "../../lib/tree/tree_func.h"

#include "lexical_analyzer.h"

#define SYN_ASSERT(quote)                                                                                       \
                    {                                                                                           \
                        fprintf (stderr, "Syntax error: " quote "\n");                                          \
                                                                                                                \
                        for (size_t i = *position; i < *position + 4 && NODE_TYPE != END;                       \
                             ++i, current_node = (token_struct -> node_array)[i])                               \
                                                                                                                \
                            fprintf (stderr, "%s ",                                                             \
                                     token_struct -> char_array[token_struct -> index_node_word[i]]);           \
                                                                                                                \
                        fprintf (stderr, "\n");                                                                 \
                        return NULL;                                                                            \
                    }

TreeNode *GetGrammar (const LanguageToken *token_struct);

TreeNode *GetFunction (const LanguageToken *token_struct, size_t *position);

TreeNode *GetLangOperator (const LanguageToken *token_struct, size_t *position);

TreeNode *GetIf (const LanguageToken *token_struct, size_t *position);

TreeNode *GetWhile (const LanguageToken *token_struct, size_t *position);

TreeNode *GetExpression (const LanguageToken *token_struct, size_t *position);

TreeNode *GetComparison (const LanguageToken *token_struct, size_t *position);

TreeNode *GetAssign (const LanguageToken *token_struct, size_t *position);

TreeNode *GetSumSub (const LanguageToken *token_struct, size_t *position);

TreeNode *GetMulDiv (const LanguageToken *token_struct, size_t *position);

TreeNode *GetPow (const LanguageToken *token_struct, size_t *position);

TreeNode *GetParenthesis (const LanguageToken *token_struct, size_t *position);

TreeNode *GetNum (const LanguageToken *token_struct, size_t *position);

TreeNode *GetVar (const LanguageToken *token_struct, size_t *position);

#endif
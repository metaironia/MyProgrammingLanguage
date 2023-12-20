#ifndef SYNTAX_ANALYZER_H
#define SYNTAX_ANALYZER_H

#include "../../lib/tree/tree_func.h"

#include "lexical_analyzer.h"

TreeNode *GetGrammar (const LanguageToken *token_struct);

TreeNode *GetSumSub (const LanguageToken *token_struct, size_t *position);

TreeNode *GetMulDiv (const LanguageToken *token_struct, size_t *position);

TreeNode *GetPow (const LanguageToken *token_struct, size_t *position);

TreeNode *GetParenthesis (const LanguageToken *token_struct, size_t *position);

TreeNode *GetNum (const LanguageToken *token_struct, size_t *position);

TreeNode *GetVar (const LanguageToken *token_struct, size_t *position);

#endif
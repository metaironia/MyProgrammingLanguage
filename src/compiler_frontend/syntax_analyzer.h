#ifndef SYNTAX_ANALYZER_H
#define SYNTAX_ANALYZER_H

#include "../../lib/tree/tree_func.h"

#include "lexical_analyzer.h"

#define SYN_ASSERT(condition, quote)    if (!IsSyntaxAssert (condition, quote, *position, token_struct)) \
                                            return NULL;

const int WORDS_TO_PRINT_AMOUNT = 5;

bool IsSyntaxAssert (const int condition,      const char *error_quote, 
                     const size_t current_pos, const LanguageToken *token_struct);

TreeNode *GetGrammar (const LanguageToken *token_struct);

TreeNode *GetFunction (const LanguageToken *token_struct, size_t *position);

TreeNode *GetArgument (const LanguageToken *token_struct, size_t *position);

TreeNode *GetLangOperator (const LanguageToken *token_struct, size_t *position);

TreeNode *GetIf (const LanguageToken *token_struct, size_t *position);

TreeNode *GetPrint (const LanguageToken *token_struct, size_t *position);

TreeNode *GetWhile (const LanguageToken *token_struct, size_t *position);

TreeNode *GetExpression (const LanguageToken *token_struct, size_t *position);

TreeNode *GetComparison (const LanguageToken *token_struct, size_t *position);

TreeNode *GetAssign (const LanguageToken *token_struct, size_t *position);

TreeNode *GetSumSub (const LanguageToken *token_struct, size_t *position);

TreeNode *GetMulDiv (const LanguageToken *token_struct, size_t *position);

TreeNode *GetPow (const LanguageToken *token_struct, size_t *position);

TreeNode *GetParenthesis (const LanguageToken *token_struct, size_t *position);

TreeNode *GetFuncCall (const LanguageToken *token_struct, size_t *position);

TreeNode *GetNum (const LanguageToken *token_struct, size_t *position);

TreeNode *GetVar (const LanguageToken *token_struct, size_t *position);

#endif
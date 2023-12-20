#ifndef SYNTAX_ANALYZER_H
#define SYNTAX_ANALYZER_H

#include "../../lib/tree/tree_func.h"

#include "lexical_analyzer.h"

TreeNode *GetGrammar (const LanguageToken *token_struct);

TreeNode *GetSumSub (const LanguageToken *token_struct, size_t *position);

TreeNode *GetNumVar (const LanguageToken *token_struct, size_t *position);

#endif
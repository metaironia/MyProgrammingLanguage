#include <assert.h>

#include "../../lib/tree/tree_func.h"
#include "../../lib/tree/tree_log.h"

#include "../../lib/tree/math_tree/dsl_commands.h"
#include "../../lib/tree/math_tree/math_tree_func.h"

#include "syntax_analyzer.h"

void SyntaxAssert (const char *error_quote, const size_t current_pos, const LanguageToken *token_struct) {

    if (!error_quote)
        return;

    fprintf (stderr, "Syntax error: %s\n", error_quote);

    size_t pos_to_print    = (current_pos >= 1) ? (current_pos - 1) : 0;
    TreeNode* current_node = (token_struct -> node_array)[pos_to_print];

    while (pos_to_print < current_pos + WORDS_TO_PRINT_AMOUNT) {

        if (NODE_TYPE == END)
            break;

        fprintf (stderr, "%s ", token_struct -> char_array[token_struct -> index_node_word[pos_to_print]]);

        current_node = (token_struct -> node_array)[++pos_to_print];
    }

    fprintf (stderr, "\n");
}

TreeNode *GetGrammar (const LanguageToken *token_struct) {

    assert (token_struct);
    assert (token_struct -> node_array);

    size_t position = 0;

    TreeNode *tree_node    = NEW_FUNC_ (INIT_FUNC_INT_ (GetFunction (token_struct, &position)));
    TreeNode **next_node   = &(tree_node -> left_branch);

    TreeNode *current_node = (token_struct -> node_array)[position];

    while (NODE_TYPE != END && tree_node -> right_branch != NULL) {

        if (*next_node) {

            *next_node = NEW_FUNC_ (*next_node);
            next_node  = &((*next_node) -> left_branch);
        }

        if (!(*next_node = GetFunction (token_struct, &position)))
            return NULL;

        *next_node = INIT_FUNC_INT_ (*next_node);

        current_node = (token_struct -> node_array)[position];
    }

    if (((token_struct -> node_array)[position]) -> data -> nodeType != END)
        return NULL;

    return tree_node;
}

TreeNode *GetFunction (const LanguageToken *token_struct, size_t *position) {

    assert (position);
    assert (token_struct);
    assert ((token_struct -> node_array)[*position]);

    const size_t old_position = *position;
    TreeNode *current_node    = (token_struct -> node_array)[*position];

    TreeNode *tree_node = GetVar (token_struct, position);

    SYN_ASSERT (old_position != *position, "WRONG FUNCTION NAME");

    current_node = (token_struct -> node_array)[*position];

    tree_node -> left_branch = GetArgument (token_struct, position);

    current_node = (token_struct -> node_array)[*position];

    SYN_ASSERT (NODE_TYPE == LANGUAGE_OPERATOR && NODE_LANG_OPERATOR == OPEN_BRACE,
                "NO OPEN BRACE after FUNCTION DECLARATION");

    (*position)++;

    TreeNode **next_node = &(tree_node -> right_branch);

    while (!(NODE_TYPE == LANGUAGE_OPERATOR && NODE_LANG_OPERATOR == END_LINE)) {

        *next_node = GetLangOperator (token_struct, position);

        if (!(*next_node))
            return NULL;

        next_node  = &((*next_node) -> right_branch);

        current_node = (token_struct -> node_array)[*position];
    }

    SYN_ASSERT (NODE_TYPE == LANGUAGE_OPERATOR && NODE_LANG_OPERATOR == END_LINE,
                "ERROR in FUNCTION END LINE");

    (*position)++;

    return tree_node;
}

TreeNode *GetArgument (const LanguageToken *token_struct, size_t *position) {

    assert (position);
    assert (token_struct);
    assert ((token_struct -> node_array)[*position]);

    TreeNode *current_node = (token_struct -> node_array)[*position];

    if (NODE_TYPE == LANGUAGE_OPERATOR && NODE_LANG_OPERATOR == OPEN_BRACE)
        return NULL;

    if (NODE_TYPE != LANGUAGE_OPERATOR || NODE_LANG_OPERATOR != FUNC_ARG)
        return NULL;

    (*position)++;

    TreeNode *tree_node  = INIT_ (TYPE_INT_, GetVar (token_struct, position));
    current_node         = (token_struct -> node_array)[*position];

    while (NODE_TYPE == VARIABLE) {

        tree_node    = COMMA_ (INIT_ (TYPE_INT_, GetVar (token_struct, position)), tree_node);
        current_node = (token_struct -> node_array)[*position];
    }

    return tree_node;
}

TreeNode *GetLangOperator (const LanguageToken *token_struct, size_t *position) {

    assert (position);
    assert (token_struct);
    assert ((token_struct -> node_array)[*position]);

    const size_t old_position = *position;
    TreeNode *tree_node       = GetIf (token_struct, position);

    if (old_position == *position)
        tree_node = GetWhile (token_struct, position);

    if (old_position == *position)
        tree_node = GetAssign (token_struct, position);

    if (old_position == *position)
        tree_node = GetReturn (token_struct, position);

    if (old_position == *position)
        tree_node = GetPrint (token_struct, position);

    if (old_position == *position)
        tree_node = GetFuncCall (token_struct, position);

    SYN_ASSERT (old_position != *position && tree_node, "ERROR IN LANG OPERATOR");

    (*position)++;

    return END_LINE_ (tree_node);
}

TreeNode *GetReturn (const LanguageToken *token_struct, size_t *position) {

    assert (position);
    assert (token_struct);
    assert ((token_struct -> node_array)[*position]);

    TreeNode *current_node = (token_struct -> node_array)[*position];

    if (NODE_TYPE != LANGUAGE_OPERATOR || NODE_LANG_OPERATOR != FUNC_RET)
        return NULL;

    (*position)++;

    TreeNode *tree_node = FUNC_RET_ (GetExpression (token_struct, position));

    SYN_ASSERT (tree_node -> left_branch, "ERROR in EXPRESSION in RETURN");

    return tree_node;
}

TreeNode *GetPrint (const LanguageToken *token_struct, size_t *position) {

    assert (position);
    assert (token_struct);
    assert ((token_struct -> node_array)[*position]);

    TreeNode *current_node = (token_struct -> node_array)[*position];

    if (NODE_TYPE != LANGUAGE_OPERATOR || NODE_LANG_OPERATOR != PRINT)
        return NULL;

    TreeNode *tree_node = PRINT_;

    (*position)++;

    current_node = (token_struct -> node_array)[*position];

    SYN_ASSERT (NODE_TYPE == VARIABLE, "WRONG EXPRESSION after PRINT");

    tree_node -> left_branch = GetVar (token_struct, position);

    return tree_node;
}

TreeNode *GetIf (const LanguageToken *token_struct, size_t *position) {

    assert (position);
    assert (token_struct);
    assert ((token_struct -> node_array)[*position]);

    TreeNode *current_node = (token_struct -> node_array)[*position];

    if (NODE_TYPE != LANGUAGE_OPERATOR || NODE_LANG_OPERATOR != IF)
        return NULL;

    (*position)++;

    TreeNode *tree_node = IF_ (GetExpression (token_struct, position));

    SYN_ASSERT (tree_node -> left_branch, "ERROR in IF in EXPRESSION");

    current_node = (token_struct -> node_array)[*position];

    SYN_ASSERT (NODE_TYPE == LANGUAGE_OPERATOR && NODE_LANG_OPERATOR == OPEN_BRACE,
                "ERROR in IF after EXPRESSION");

    (*position)++;

    tree_node -> right_branch = GetLangOperator (token_struct, position);

    return tree_node;
}

TreeNode *GetWhile (const LanguageToken *token_struct, size_t *position) {

    assert (position);
    assert (token_struct);
    assert ((token_struct -> node_array)[*position]);

    TreeNode *current_node = (token_struct -> node_array)[*position];

    if (NODE_TYPE != LANGUAGE_OPERATOR || NODE_LANG_OPERATOR != WHILE)
        return NULL;

    (*position)++;

    TreeNode *tree_node = WHILE_ (GetExpression (token_struct, position));

    SYN_ASSERT (tree_node -> left_branch, "ERROR in WHILE in EXPRESSION");

    current_node = (token_struct -> node_array)[*position];

    SYN_ASSERT (NODE_TYPE == LANGUAGE_OPERATOR && NODE_LANG_OPERATOR == OPEN_BRACE,
                "ERROR in WHILE after EXPRESSION");

    (*position)++;

    tree_node -> right_branch = GetLangOperator (token_struct, position);

    return tree_node;
}

TreeNode *GetExpression (const LanguageToken *token_struct, size_t *position) {

    assert (position);
    assert (token_struct);
    assert ((token_struct -> node_array)[*position]);

    TreeNode *tree_node    = GetComparison (token_struct, position);
    TreeNode *current_node = (token_struct -> node_array)[*position];

    SYN_ASSERT (tree_node, NULL);

    while (NODE_TYPE == LANGUAGE_OPERATOR &&
          (NODE_LANG_OPERATOR == OR || NODE_LANG_OPERATOR == AND)) {

        (*position)++;

        switch (NODE_LANG_OPERATOR) {

            case AND:
                tree_node = AND_ (tree_node, GetComparison (token_struct, position));
                break;

            case OR:
                tree_node = OR_ (tree_node, GetComparison (token_struct, position));
                break;

            default:
                SYN_ASSERT (0, "UNKNOWN ERROR in EXPRESSION.\n");
        }

        SYN_ASSERT (tree_node -> right_branch, "ERROR after OR/AND OPERATOR");

        current_node = (token_struct -> node_array)[*position];
    }

    return tree_node;
}

TreeNode *GetComparison (const LanguageToken *token_struct, size_t *position) {

    assert (position);
    assert (token_struct);
    assert ((token_struct -> node_array)[*position]);

    TreeNode *tree_node    = GetSumSub (token_struct, position);
    TreeNode *current_node = (token_struct -> node_array)[*position];

    SYN_ASSERT (tree_node, NULL);

    current_node = (token_struct -> node_array)[*position];

    if (NODE_TYPE != BINARY_OPERATOR)
        return tree_node;

    (*position)++;

    switch (NODE_MATH_OPERATOR) {

        case OPERATOR_EQUAL:
            tree_node = EQUAL_ (tree_node, GetSumSub (token_struct, position));
            break;

        case OPERATOR_NOT_EQUAL:
            tree_node = NOT_EQUAL_ (tree_node, GetSumSub (token_struct, position));
            break;

        case OPERATOR_GREATER:
            tree_node = GREATER_ (tree_node, GetSumSub (token_struct, position));
            break;

        case OPERATOR_LESS:
            tree_node = LESS_ (tree_node, GetSumSub (token_struct, position));
            break;

        default:
            (*position)--;
            break;
    }

    SYN_ASSERT (tree_node -> right_branch, "ERROR after COMPARISON OPERATOR");

    return tree_node;
}

TreeNode *GetAssign (const LanguageToken *token_struct, size_t *position) {

    assert (position);
    assert (token_struct);
    assert ((token_struct -> node_array)[*position]);

    size_t old_position = *position;

    TreeNode *tree_node    = ASSIGN_;
    TreeNode *current_node = (token_struct -> node_array)[*position];

    bool is_init = false;

    if (NODE_TYPE == LANGUAGE_OPERATOR && NODE_LANG_OPERATOR == INIT) {

        is_init = true;
        (*position)++;
    }

    tree_node -> left_branch = GetVar (token_struct, position);

    if (old_position == *position)
        return NULL;

    current_node = (token_struct -> node_array)[*position];

    SYN_ASSERT (NODE_TYPE == LANGUAGE_OPERATOR && NODE_LANG_OPERATOR == ASSIGN,
                "ERROR in ASSIGNMENT OPERATOR");

    (*position)++;

    old_position = *position;

    tree_node -> right_branch = GetSumSub (token_struct, position);

    current_node = (token_struct -> node_array)[*position];

    if (!(tree_node -> right_branch)) {

        if (NODE_TYPE == LANGUAGE_OPERATOR && NODE_LANG_OPERATOR == READ) {

            tree_node -> right_branch = READ_;
            (*position)++;
        }

        else
            SYN_ASSERT (0, "ERROR after ASSIGNMENT");
    }

    if (is_init)
        tree_node = INIT_ (TYPE_INT_, tree_node);

    return tree_node;
}

TreeNode *GetSumSub (const LanguageToken *token_struct, size_t *position) {

    assert (position);
    assert (token_struct);
    assert ((token_struct -> node_array)[*position]);

    TreeNode *tree_node    = GetMulDiv (token_struct, position);
    TreeNode *current_node = (token_struct -> node_array)[*position];

    SYN_ASSERT (tree_node, NULL);

    while (NODE_TYPE == BINARY_OPERATOR &&
          (NODE_MATH_OPERATOR == OPERATOR_ADD || NODE_MATH_OPERATOR == OPERATOR_SUB)) {

        (*position)++;

        switch (NODE_MATH_OPERATOR) {

            case OPERATOR_ADD:
                tree_node = ADD_ (tree_node, GetMulDiv (token_struct, position));
                break;

            case OPERATOR_SUB:
                tree_node = SUB_ (tree_node, GetMulDiv (token_struct, position));
                break;

            default:
                SYN_ASSERT (0, "UNKNOWN ERROR in SUM or SUB COMPUTING.\n");
        }

        SYN_ASSERT (tree_node -> right_branch, "ERROR after SUB or ADD OPERATOR");

        current_node = (token_struct -> node_array)[*position];
    }

    return tree_node;
}

TreeNode *GetMulDiv (const LanguageToken *token_struct, size_t *position) {

    assert (position);
    assert (token_struct);
    assert ((token_struct -> node_array)[*position]);

    TreeNode *tree_node    = GetPow (token_struct, position);
    TreeNode *current_node = (token_struct -> node_array)[*position];

    SYN_ASSERT (tree_node, NULL);

    while (NODE_TYPE == BINARY_OPERATOR &&
          (NODE_MATH_OPERATOR == OPERATOR_MUL || NODE_MATH_OPERATOR == OPERATOR_DIV)) {

        (*position)++;

        switch (NODE_MATH_OPERATOR) {

            case OPERATOR_MUL:
                tree_node = MUL_ (tree_node, GetPow (token_struct, position));
                break;

            case OPERATOR_DIV:
                tree_node = DIV_ (tree_node, GetPow (token_struct, position));
                break;

            default:
                SYN_ASSERT (0, "UNKNOWN ERROR in MUL or DIV COMPUTING.\n");
        }

        SYN_ASSERT (tree_node -> right_branch, "ERROR after MUL/DIV OPERATOR");

        current_node = (token_struct -> node_array)[*position];
    }

    return tree_node;
}

TreeNode *GetPow (const LanguageToken *token_struct, size_t *position) {

    assert (position);
    assert (token_struct);
    assert ((token_struct -> node_array)[*position]);

    TreeNode *tree_node    = GetParenthesis (token_struct, position);
    TreeNode *current_node = (token_struct -> node_array)[*position];

    SYN_ASSERT (tree_node, NULL);

    while (NODE_TYPE == BINARY_OPERATOR && NODE_MATH_OPERATOR == OPERATOR_POW) {

        (*position)++;

        switch (NODE_MATH_OPERATOR) {

            case OPERATOR_POW:
                tree_node = POW_ (tree_node, GetParenthesis (token_struct, position));
                break;

            default:
                SYN_ASSERT (0, "UNKNOWN ERROR in POW COMPUTING.\n");
        }

        current_node = (token_struct -> node_array)[*position];
    }

    return tree_node;
}

TreeNode *GetParenthesis (const LanguageToken *token_struct, size_t *position) {

    assert (position);
    assert (token_struct);
    assert ((token_struct -> node_array)[*position]);

    TreeNode *tree_node    = NULL;
    TreeNode *current_node = (token_struct -> node_array)[*position];

    if (NODE_TYPE == UNARY_OPERATOR && NODE_MATH_OPERATOR == OPERATOR_OPEN_PARENTHESIS) {

        (*position)++;

        tree_node = GetSumSub (token_struct, position);

        SYN_ASSERT (tree_node, NULL);

        current_node = (token_struct -> node_array)[*position];

        SYN_ASSERT (NODE_TYPE == UNARY_OPERATOR && NODE_MATH_OPERATOR == OPERATOR_CLOSE_PARENTHESIS,
                    "NO CLOSE BRACKET");

        (*position)++;
    }

    else
        switch (NODE_TYPE) {

            case NUMBER:
                tree_node = GetNum (token_struct, position);
                break;

            case VARIABLE:
                tree_node = GetVar (token_struct, position);
                break;

            case LANGUAGE_OPERATOR:
                if (NODE_LANG_OPERATOR == FUNC_CALL)
                    tree_node = GetFuncCall (token_struct, position);
                break;

            default:
                return NULL;
        }

    return tree_node;
}

TreeNode *GetFuncCall (const LanguageToken *token_struct, size_t *position) {

    assert (position);
    assert (token_struct);
    assert ((token_struct -> node_array)[*position]);

    TreeNode *current_node = (token_struct -> node_array)[*position];

    if (!(NODE_TYPE == LANGUAGE_OPERATOR && NODE_LANG_OPERATOR == FUNC_CALL))
        return NULL;

    (*position)++;

    TreeNode *tree_node  = FUNC_CALL_ (GetVar (token_struct, position));

    SYN_ASSERT (tree_node -> left_branch, "WRONG FUNCTION NAME in FUNCTION CALL");

    TreeNode **next_node = &(tree_node -> left_branch -> left_branch);
    current_node         = (token_struct -> node_array)[*position];

    while (NODE_TYPE == VARIABLE || NODE_TYPE == NUMBER) {

        if (*next_node != NULL) {

            *next_node = COMMA_ (NULL, *next_node);
            next_node  = &((*next_node) -> left_branch);
        }

        switch (NODE_TYPE) {

            case VARIABLE:
                *next_node = GetVar (token_struct, position);
                break;

            case NUMBER:
                *next_node = GetNum (token_struct, position);
                break;

            default:
                *next_node = NULL;
        }

        current_node = (token_struct -> node_array)[*position];
    }

    return tree_node;
}

TreeNode *GetVar (const LanguageToken *token_struct, size_t *position) {

    assert (position);
    assert (token_struct);
    assert ((token_struct -> node_array)[*position]);

    TreeNode *current_node = (token_struct -> node_array)[*position];

    switch (NODE_TYPE) {

        case VARIABLE:
            (*position)++;
            break;

        default:
            return NULL;
    }

    return current_node;
}

TreeNode *GetNum (const LanguageToken *token_struct, size_t *position) {

    assert (position);
    assert (token_struct);
    assert ((token_struct -> node_array)[*position]);

    TreeNode *current_node = (token_struct -> node_array)[*position];

    switch (NODE_TYPE) {

        case NUMBER:
            (*position)++;
            break;

        default:
            return NULL;
    }

    return current_node;
}

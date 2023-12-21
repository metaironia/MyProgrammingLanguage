#include <assert.h>

#include "../../lib/tree/tree_func.h"
#include "../../lib/tree/tree_log.h"

#include "../../lib/tree/math_tree/dsl_commands.h"
#include "../../lib/tree/math_tree/math_tree_func.h"

#include "syntax_analyzer.h"
//TODO add syn asserts

TreeNode *GetGrammar (const LanguageToken *token_struct) {

    assert (token_struct);
    assert (token_struct -> node_array);

    size_t position = 0;

    TreeNode *tree_node = GetLangOperator (token_struct, &position);
    TreeNode **next_node = &(tree_node -> right_branch);

    TreeNode *current_node = (token_struct -> node_array)[position]; 

    while (NODE_TYPE != END) {

        *next_node = GetLangOperator (token_struct, &position);
        next_node = &((*next_node) -> right_branch);

        current_node = (token_struct -> node_array)[position];
    }

    if (((token_struct -> node_array)[position]) -> data -> nodeType != END) {

        fprintf (stderr, "err gram\n");
        return NULL;
    }

    return tree_node;
}

TreeNode *GetLangOperator (const LanguageToken *token_struct, size_t *position) {
    
    assert (token_struct);
    assert ((token_struct -> node_array)[*position]);
    assert (position);

    const size_t old_position = *position;

    TreeNode *tree_node = GetIf (token_struct, position);

    if (old_position == *position)          
        tree_node = GetAssign (token_struct, position); 

    if (old_position == *position) {

        fprintf (stderr, "ERROR IN GET LANG OPERATOR\n");
        return NULL;
    }  

    TreeNode *current_node = (token_struct -> node_array)[*position];     

    if (NODE_TYPE != LANGUAGE_OPERATOR || NODE_LANG_OPERATOR != END_LINE) {

        fprintf (stderr, "ERROR in END LINE\n");
        return NULL;
    }

    (*position)++;

    return END_LINE_ (tree_node);
}

TreeNode *GetIf (const LanguageToken *token_struct, size_t *position) {

    assert (token_struct);
    assert ((token_struct -> node_array)[*position]);
    assert (position);

    TreeNode *current_node = (token_struct -> node_array)[*position];   

    if (NODE_TYPE != LANGUAGE_OPERATOR || NODE_LANG_OPERATOR != IF) {

        fprintf (stderr, "ERROR in IF WORD\n");
        return NULL;  
    }

    (*position)++;

    TreeNode *tree_node       = IF_;
    tree_node -> left_branch  = GetExpression (token_struct, position);

    current_node = (token_struct -> node_array)[*position];

    if (NODE_TYPE != LANGUAGE_OPERATOR || NODE_LANG_OPERATOR != OPEN_BRACE) {
       
        fprintf (stderr, "ERROR in IF after EXPRESSION\n");
        return NULL;  
    }

    (*position)++;

    tree_node -> right_branch = GetLangOperator (token_struct, position);

    return tree_node;
}

TreeNode *GetExpression (const LanguageToken *token_struct, size_t *position) {

    assert (token_struct);
    assert ((token_struct -> node_array)[*position]);
    assert (position);

    TreeNode *tree_node    = GetComparison (token_struct, position);
    TreeNode *current_node = (token_struct -> node_array)[*position];

    while (NODE_TYPE == LANGUAGE_OPERATOR && 
          (NODE_LANG_OPERATOR == OR || NODE_LANG_OPERATOR == AND)) {

        (*position)++;

        switch (NODE_MATH_OPERATOR) {

            case AND:
                tree_node = AND_ (tree_node, GetComparison (token_struct, position));
                break;

            case OR:
                tree_node = OR_ (tree_node, GetComparison (token_struct, position));
                break;
            
            default:
                fprintf (stderr, "UNKNOWN ERROR in EXPRESSION.\n");
                return NULL;
        }

        current_node = (token_struct -> node_array)[*position];
    }

    return tree_node;
}

TreeNode *GetComparison (const LanguageToken *token_struct, size_t *position) {

    assert (token_struct);
    assert ((token_struct -> node_array)[*position]);
    assert (position);

    TreeNode *tree_node    = GetSumSub (token_struct, position);
    TreeNode *current_node = (token_struct -> node_array)[*position];

    if (NODE_TYPE != BINARY_OPERATOR || 
        (NODE_LANG_OPERATOR != OPERATOR_EQUAL && NODE_LANG_OPERATOR != OPERATOR_NOT_EQUAL &&
        NODE_LANG_OPERATOR != OPERATOR_GREATER && NODE_LANG_OPERATOR != OPERATOR_LESS)) {
        
        fprintf (stderr, "ERROR in COMPARISON\n");
        return NULL;
    }

    (*position)++;

    switch (NODE_LANG_OPERATOR) {

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
            fprintf (stderr, "UNKNOWN ERROR in COMPARISON\n");
            return NULL;
    }  

    return tree_node;
}

TreeNode *GetAssign (const LanguageToken *token_struct, size_t *position) {

    assert (token_struct);
    assert ((token_struct -> node_array)[*position]);
    assert (position);

    TreeNode *tree_node    = ASSIGN_;
    TreeNode *current_node = (token_struct -> node_array)[*position];

    bool is_init = false;

    if (NODE_TYPE == LANGUAGE_OPERATOR && NODE_LANG_OPERATOR == INIT) {
        
        is_init = true;
        (*position)++;
    }     

    tree_node -> left_branch = GetVar (token_struct, position);

    current_node = (token_struct -> node_array)[*position];

    if (NODE_TYPE != LANGUAGE_OPERATOR || NODE_LANG_OPERATOR != ASSIGN) {

        fprintf (stderr, "ERROR in ASSIGN\n");
        return NULL;
    }

    (*position)++;

    const size_t old_position = *position;

    tree_node -> right_branch = GetSumSub (token_struct, position);

    current_node = (token_struct -> node_array)[*position];

    if (old_position == *position) {
        
        if (NODE_TYPE == LANGUAGE_OPERATOR && NODE_LANG_OPERATOR == READ) {
           
            tree_node -> right_branch = READ_;
            (*position)++;
        }

        else {

            fprintf (stderr, "ERROR after ASSIGN.\n");
            return NULL;    
        }
    }

    if (is_init)
        tree_node = INIT_ (TYPE_INT_, tree_node);

    return tree_node;
} 

TreeNode *GetSumSub (const LanguageToken *token_struct, size_t *position) {

    assert (token_struct);
    assert ((token_struct -> node_array)[*position]);
    assert (position);

    TreeNode *tree_node    = GetMulDiv (token_struct, position);
    TreeNode *current_node = (token_struct -> node_array)[*position];

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
                fprintf (stderr, "UNKNOWN ERROR in SUM or SUB COMPUTING.\n");
                return NULL;
        }

        current_node = (token_struct -> node_array)[*position];
    }

    return tree_node;
}

TreeNode *GetMulDiv (const LanguageToken *token_struct, size_t *position) {

    assert (token_struct);
    assert ((token_struct -> node_array)[*position]);
    assert (position);

    TreeNode *tree_node    = GetPow (token_struct, position);
    TreeNode *current_node = (token_struct -> node_array)[*position];

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
                fprintf (stderr, "UNKNOWN ERROR in MUL or DIV COMPUTING.\n");
                return NULL;
        }

        current_node = (token_struct -> node_array)[*position];
    }

    return tree_node;
}

TreeNode *GetPow (const LanguageToken *token_struct, size_t *position) {

    assert (token_struct);
    assert ((token_struct -> node_array)[*position]);
    assert (position);

    TreeNode *tree_node    = GetParenthesis (token_struct, position);
    TreeNode *current_node = (token_struct -> node_array)[*position];

    while (NODE_TYPE == BINARY_OPERATOR && NODE_MATH_OPERATOR == OPERATOR_POW) {

        (*position)++;

        switch (NODE_MATH_OPERATOR) {

            case OPERATOR_POW:
                tree_node = POW_ (tree_node, GetParenthesis (token_struct, position));
                break;
            
            default:
                fprintf (stderr, "UNKNOWN ERROR in POW COMPUTING.\n");
                return NULL;
        }

        current_node = (token_struct -> node_array)[*position];
    }

    return tree_node;
}

TreeNode *GetParenthesis (const LanguageToken *token_struct, size_t *position) {

    assert (token_struct);
    assert ((token_struct -> node_array)[*position]);
    assert (position);

    TreeNode *tree_node    = NULL;
    TreeNode *current_node = (token_struct -> node_array)[*position];

    if (NODE_TYPE == UNARY_OPERATOR && NODE_MATH_OPERATOR == OPERATOR_OPEN_PARENTHESIS) {

        (*position)++;

        tree_node = GetSumSub (token_struct, position);

        current_node = (token_struct -> node_array)[*position];

        if (NODE_TYPE != UNARY_OPERATOR || NODE_MATH_OPERATOR != OPERATOR_CLOSE_PARENTHESIS) {

            fprintf (stderr, "make syn assert close bracket\n");
            return NULL;
        }

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
            
            default:
                fprintf (stderr, "UNKNOWN ERROR in PARENTHESIS FIND\n");
                return NULL;
        }

    return tree_node;
}


TreeNode *GetVar (const LanguageToken *token_struct, size_t *position) {

    assert (token_struct);
    assert ((token_struct -> node_array)[*position]);
    assert (position);

    TreeNode *current_node = (token_struct -> node_array)[(*position)++];

    switch (NODE_TYPE) {

        case VARIABLE:
            break;

        default:
            fprintf (stderr, "VAR ERR\n");
            return NULL;
    }

    return current_node;
}
TreeNode *GetNum (const LanguageToken *token_struct, size_t *position) {

    assert (token_struct);
    assert ((token_struct -> node_array)[*position]);
    assert (position);

    TreeNode *current_node = (token_struct -> node_array)[(*position)++];

    switch (NODE_TYPE) {

        case NUMBER:
            break;

        default:
            fprintf (stderr, "NUM ERR\n");
            return NULL;
    }

    return current_node;
}
#include <assert.h>

#include "../../lib/tree/tree_func.h"
#include "../../lib/tree/tree_log.h"

#include "../../lib/tree/math_tree/dsl_commands.h"
#include "../../lib/tree/math_tree/math_tree_func.h"

#include "syntax_analyzer.h"
//TODO add verifiers
TreeNode *GetGrammar (const LanguageToken *token_struct) {

    assert (token_struct);
    assert (token_struct -> node_array);

    size_t position = 0;

    TreeNode *tree_node = GetSumSub (token_struct, &position); 

    if (((token_struct -> node_array)[position]) -> data -> nodeType != END)
        fprintf (stderr, "err gram\n");

    return tree_node;
}

TreeNode *GetSumSub (const LanguageToken *token_struct, size_t *position) {

    assert (token_struct);
    assert ((token_struct -> node_array)[*position]);
    assert (position);

    TreeNode *tree_node    = GetNumVar (token_struct, position);
    TreeNode *current_node = (token_struct -> node_array)[*position];

    while (NODE_TYPE == BINARY_OPERATOR && 
          (NODE_MATH_OPERATOR == OPERATOR_ADD || NODE_MATH_OPERATOR == OPERATOR_SUB)) {

        (*position)++;

        switch (NODE_MATH_OPERATOR) {

            case OPERATOR_ADD:
                tree_node = ADD_ (tree_node, GetNumVar (token_struct, position));
                break;

            case OPERATOR_SUB:
                tree_node = SUB_ (tree_node, GetNumVar (token_struct, position));
                break;
            
            default:
                break;
        }

    current_node = (token_struct -> node_array)[*position];
    }

    return tree_node;
}

TreeNode *GetNumVar (const LanguageToken *token_struct, size_t *position) {

    assert (token_struct);
    assert ((token_struct -> node_array)[*position]);
    assert (position);

    TreeNode *current_node = (token_struct -> node_array)[(*position)++];

    if (NODE_TYPE != NUMBER) {

        fprintf (stderr, "NUMVAR ERR\n");
        return NULL;
    }

    return current_node;
}
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "../../lib/tree/tree_func.h"

#include "../../lib/tree/math_tree/dsl_commands.h"
#include "../../lib/tree/math_tree/math_tree_func.h"

#include "lexical_analyzer.h"
#include "lexical_quotations.h"

LexicalFuncStatus LexicalAnalyzer (FILE *input_file, LanguageToken *token_struct, NameTable *name_table) {

    assert (input_file);
    assert (token_struct);
    assert (name_table);

    token_struct -> char_array = (char **) calloc (MAX_PROGRAM_LENGTH, sizeof (char *));

    StringInputFromFile (input_file, token_struct -> char_array);

    StringTokenSeparate (token_struct, name_table);

    return LEXICAL_FUNC_STATUS_OK;
}

LexicalFuncStatus StringInputFromFile (FILE *input_file, char **input_array) {

    assert (input_file);
    assert (input_array);

    char current_word[MAX_WORD_LENGTH] = ""; 

    for (size_t i = 0; !feof (input_file); i++) { 

        if (fscanf (input_file, " %[^ ,;.-\r\n]", current_word) == 0)
            current_word[0] = (char) fgetc (input_file); 

        if (current_word[0] != '\0') 
            input_array[i] = strdup (current_word);

        memset (current_word, 0, (sizeof (current_word) / sizeof (current_word[0])));
    }

    return LEXICAL_FUNC_STATUS_OK;
}

LexicalFuncStatus StringTokenSeparate (LanguageToken *token_struct, NameTable *name_table) {

    assert (token_struct);
    assert (token_struct -> char_array);
    assert (name_table);

    (token_struct -> node_array) = (TreeNode **) calloc (MAX_PROGRAM_LENGTH, sizeof (TreeNode *));  
    (token_struct -> index_node_word) = (size_t *) calloc (MAX_PROGRAM_LENGTH, sizeof (size_t));

    TreeNode **current_node = (token_struct -> node_array);

    char *current_word = (token_struct -> char_array)[0];

    for (size_t char_array_index = 0, node_array_index = 0; 
         (token_struct -> char_array)[char_array_index]; char_array_index++) {
        
        node_array_index = current_node - (token_struct -> node_array);
        current_word     = (token_struct -> char_array)[char_array_index];

        if (CheckIfWordIsNumber   (current_word, current_node) ||
            CheckIfWordIsVariable (current_word, current_node, name_table)) {

            current_node++;
            (token_struct -> index_node_word)[node_array_index] = char_array_index;
            continue;
        }

        bool is_success = false;

        CHECK_WORD_LANGUAGE_OP (current_word, IF);
        CHECK_WORD_LANGUAGE_OP (current_word, WHILE);
        CHECK_WORD_LANGUAGE_OP (current_word, ELSE);
        CHECK_WORD_LANGUAGE_OP (current_word, ASSIGN);
        CHECK_WORD_LANGUAGE_OP (current_word, PRINT);
        CHECK_WORD_LANGUAGE_OP (current_word, END_LINE);
        CHECK_WORD_LANGUAGE_OP (current_word, INIT);
        CHECK_WORD_LANGUAGE_OP (current_word, FUNC_CALL);
        CHECK_WORD_LANGUAGE_OP (current_word, FUNC_RET);
        CHECK_WORD_LANGUAGE_OP (current_word, OPEN_BRACE);
        CHECK_WORD_LANGUAGE_OP (current_word, CLOSE_BRACE);
        CHECK_WORD_LANGUAGE_OP (current_word, READ);
        CHECK_WORD_LANGUAGE_OP (current_word, FUNC_ARG);

        CHECK_WORD_MATH_OP     (current_word, BINARY_OPERATOR, ADD);
        CHECK_WORD_MATH_OP     (current_word, BINARY_OPERATOR, SUB);
        CHECK_WORD_MATH_OP     (current_word, BINARY_OPERATOR, DIV);
        CHECK_WORD_MATH_OP     (current_word, BINARY_OPERATOR, MUL);
        CHECK_WORD_MATH_OP     (current_word, BINARY_OPERATOR, POW);
        CHECK_WORD_MATH_OP     (current_word, BINARY_OPERATOR, EQUAL);
        CHECK_WORD_MATH_OP     (current_word, BINARY_OPERATOR, NOT_EQUAL);
        CHECK_WORD_MATH_OP     (current_word, BINARY_OPERATOR, LESS);
        CHECK_WORD_MATH_OP     (current_word, BINARY_OPERATOR, GREATER);
        CHECK_WORD_MATH_OP     (current_word, UNARY_OPERATOR,  OPEN_PARENTHESIS);
        CHECK_WORD_MATH_OP     (current_word, UNARY_OPERATOR,  CLOSE_PARENTHESIS);
    }       

    *current_node = END_;

    return LEXICAL_FUNC_STATUS_OK;
}




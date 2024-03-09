#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "../../lib/tree/tree_func.h"

#include "../../lib/tree/math_tree/dsl_commands.h"
#include "../../lib/tree/math_tree/math_tree_func.h"

#include "lexical_analyzer.h"
#include "lexical_quotations.h"


LexicalFuncStatus LangTokenCtor (LanguageToken *token_struct) {

    assert (token_struct);

    LangTokenDataCtor (token_struct);

    token_struct -> data_size     = 0;
    token_struct -> data_capacity = DEFAULT_DATA_CAPACITY;

    return LEXICAL_FUNC_STATUS_OK;
}

LexicalFuncStatus LangTokenDataCtor (LanguageToken *token_struct) {

    assert (token_struct);

    (token_struct -> data).char_array      = (char **)           calloc (DEFAULT_DATA_CAPACITY, sizeof (char *));
    (token_struct -> data).node_array      = (const TreeNode **) calloc (DEFAULT_DATA_CAPACITY, sizeof (TreeNode *));
    (token_struct -> data).index_node_word = (size_t *)          calloc (DEFAULT_DATA_CAPACITY, sizeof (size_t));

    if (!((token_struct -> data).char_array &&
          (token_struct -> data).node_array &&
          (token_struct -> data).index_node_word))

        return LEXICAL_FUNC_STATUS_FAIL;

    return LEXICAL_FUNC_STATUS_OK;
}


LexicalFuncStatus LangTokenDtor (LanguageToken *token_struct) {

    assert (token_struct);

    LangTokenDataDtor (token_struct);

    token_struct -> data_size     = LANG_TOKEN_POISON;
    token_struct -> data_capacity = LANG_TOKEN_POISON;

    return LEXICAL_FUNC_STATUS_OK;
}

LexicalFuncStatus LangTokenDataDtor (LanguageToken *token_struct) {

    assert (token_struct);

    const size_t char_array_size_bytes       = (token_struct -> data_capacity) * sizeof (char *);
    const size_t node_array_size_bytes       = (token_struct -> data_capacity) * sizeof (TreeNode *);
    const size_t index_node_array_size_bytes = (token_struct -> data_capacity) * sizeof (size_t);

    memset ((token_struct -> data).char_array,      LANG_TOKEN_POISON, char_array_size_bytes);
    memset ((token_struct -> data).node_array,      LANG_TOKEN_POISON, node_array_size_bytes);
    memset ((token_struct -> data).index_node_word, LANG_TOKEN_POISON, index_node_array_size_bytes);

    free ((token_struct -> data).char_array);
    free ((token_struct -> data).node_array);
    free ((token_struct -> data).index_node_word);

    (token_struct -> data).char_array      = NULL;
    (token_struct -> data).node_array      = NULL;
    (token_struct -> data).index_node_word = NULL;

    return LEXICAL_FUNC_STATUS_OK;
}

LexicalFuncStatus LangTokenAdd (LanguageToken *token_struct, char *token_word, 
                                const TreeNode *token_node, const size_t token_index) {

    assert (token_struct);

    if (token_struct -> data_size == token_struct -> data_capacity)
        LangTokenRecalloc (token_struct);

    ((token_struct -> data).char_array)[token_struct -> data_size]      = token_word;
    ((token_struct -> data).node_array)[token_struct -> data_size]      = token_node;
    ((token_struct -> data).index_node_word)[token_struct -> data_size] = token_index;

    (token_struct -> data_size)++;

    return LEXICAL_FUNC_STATUS_OK;
}

LexicalFuncStatus LangTokenRecalloc (LanguageToken *token_struct) {

    assert (token_struct);

    const size_t old_char_ptr_arr_bytes        = (token_struct -> data_capacity) * sizeof (char *);
    const size_t old_node_ptr_arr_bytes        = (token_struct -> data_capacity) * sizeof (TreeNode *);
    const size_t old_index_node_word_arr_bytes = (token_struct -> data_capacity) * sizeof (size_t);

    const size_t new_char_ptr_arr_bytes        = old_char_ptr_arr_bytes * INCREASE_NUM;
    const size_t new_node_ptr_arr_bytes        = old_node_ptr_arr_bytes * INCREASE_NUM;
    const size_t new_index_node_word_arr_bytes = old_index_node_word_arr_bytes * INCREASE_NUM;   

    char           **char_ptr_arr        = (token_struct -> data).char_array;
    const TreeNode **node_ptr_arr        = (token_struct -> data).node_array;
    size_t          *index_node_word_arr = (token_struct -> data).index_node_word;

    char_ptr_arr        = (char **)           realloc (char_ptr_arr,        new_char_ptr_arr_bytes);
    node_ptr_arr        = (const TreeNode **) realloc (node_ptr_arr,        new_node_ptr_arr_bytes);
    index_node_word_arr = (size_t *)          realloc (index_node_word_arr, new_index_node_word_arr_bytes);

    if (!char_ptr_arr || !node_ptr_arr || !index_node_word_arr)
        return LEXICAL_FUNC_STATUS_FAIL;

    size_t cur_data_size = token_struct -> data_size;

    memset (char_ptr_arr + cur_data_size,        0, new_char_ptr_arr_bytes - old_char_ptr_arr_bytes);
    memset (node_ptr_arr + cur_data_size,        0, new_node_ptr_arr_bytes - old_node_ptr_arr_bytes);
    memset (index_node_word_arr + cur_data_size, 0, new_index_node_word_arr_bytes - old_index_node_word_arr_bytes);

    (token_struct -> data).char_array      = char_ptr_arr;
    (token_struct -> data).node_array      = node_ptr_arr;
    (token_struct -> data).index_node_word = index_node_word_arr;

    token_struct -> data_capacity *= INCREASE_NUM;

    return LEXICAL_FUNC_STATUS_OK;
}

LexicalFuncStatus LangTokenDump (const LanguageToken *token_struct) {

    assert (token_struct);

    fprintf     (stderr, "LanguageToken struct [0x%p] {\n", token_struct);
    fprintf     (stderr, "    data size     = %zu\n", token_struct -> data_size);
    fprintf     (stderr, "    data capacity = %zu\n", token_struct -> data_capacity);

    fprintf     (stderr, "    data dump {\n");

    for (size_t i = 0; i < token_struct -> data_capacity; i++)
        fprintf (stderr, "        word = '%s', TreeNode addr = %p, word index in text = %zu\n", 
                         ((token_struct -> data).char_array)[i],
                         ((token_struct -> data).node_array)[i],
                         ((token_struct -> data).index_node_word)[i]);

    fprintf     (stderr, "    }\n"
                         "}\n");

    return LEXICAL_FUNC_STATUS_OK;
}

/*
LexicalFuncStatus LexicalAnalyzer (FILE *input_file, LanguageToken *token_struct, NameTable *name_table) {

    assert (input_file);
    assert (token_struct);
    assert (name_table);

    token_struct -> char_array      = (char **)     calloc (MAX_PROGRAM_LENGTH, sizeof (char *));

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

    (token_struct -> node_array)      = (TreeNode **) calloc (MAX_PROGRAM_LENGTH, sizeof (TreeNode *));
    (token_struct -> index_node_word) = (size_t *)    calloc (MAX_PROGRAM_LENGTH, sizeof (size_t));

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
//        CHECK_WORD_LANGUAGE_OP (current_word, ELSE);
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
        CHECK_WORD_LANGUAGE_OP (current_word, AND);
        CHECK_WORD_LANGUAGE_OP (current_word, OR);

        CHECK_WORD_MATH_OP     (current_word, BINARY_OPERATOR, ADD);
        CHECK_WORD_MATH_OP     (current_word, BINARY_OPERATOR, SUB);
        CHECK_WORD_MATH_OP     (current_word, BINARY_OPERATOR, DIV);
        CHECK_WORD_MATH_OP     (current_word, BINARY_OPERATOR, MUL);
        CHECK_WORD_MATH_OP     (current_word, BINARY_OPERATOR, POW);
        CHECK_WORD_MATH_OP     (current_word, UNARY_OPERATOR,  SQRT);
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
*/

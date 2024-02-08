#include <stdio.h>
#include <assert.h>
#include <locale.h>

#include "../../lib/tree/tree_func.h"

#include "../../lib/oneginlib/functions_for_input.h"
#include "../../lib/oneginlib/functions_for_output.h"

#include "frontend_arg_checker.h"
#include "lexical_quotations.h"
#include "lexical_analyzer.h"
#include "syntax_analyzer.h"

int main (const int argc, const char *argv[]) {

    if (FrontendCheckCmdArgs (argc) == FRONTEND_STATUS_FAIL)
        return -1;

    setlocale (LC_ALL, ".1251");

    LanguageToken lang_tokens = {};

    NameTable lang_name_table = {};
    NameTableCtor (&lang_name_table);


    FILE *input_file = fopen (InputFileName (argv), "r");
    assert (input_file);

    LexicalAnalyzer (input_file, &lang_tokens, &lang_name_table);

    fclose (input_file);
    input_file = NULL;

    /*
    for (size_t i = 0; test_token.node_array[i]; i++) {
        NodeTypePrint (stderr, test_token.node_array[i], &test_name_table);
        fprintf (stderr, "|");
        fprintf (stderr, "%s", (test_token.char_array)[test_token.index_node_word[i]]);
        fprintf (stderr, "|\n");
    }
    fprintf (stderr, "\ngde\n");
    */

    Tree lang_tree = {};
    TreeCtor (&lang_tree);

    lang_tree.root = GetGrammar (&lang_tokens);

    if (!lang_tree.root)
        return -1;

    FILE *output_file = fopen (OutputFileName (argv), "w");
    assert (output_file);

    LangTreeFilePrint (output_file, &lang_tree, &lang_name_table);

    fclose (output_file);
    output_file = NULL;

    TreeDtor (&lang_tree);
    NameTableDtor (&lang_name_table);

    return 0;
}

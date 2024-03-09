#include <stdio.h>
#include <assert.h>
#include <locale.h>

#include "../../lib/tree/tree_func.h"
#include "../../lib/tree/tree_log.h"

#include "../../lib/oneginlib/functions_for_input.h"
#include "../../lib/oneginlib/functions_for_output.h"

#include "frontend_arg_checker.h"
#include "lexical_quotations.h"
#include "lexical_analyzer.h"
#include "syntax_analyzer.h"
#include "semantic_analyzer.h"

int main (const int argc, const char *argv[]) {

    if (FrontendCheckCmdArgs (argc) == FRONTEND_STATUS_FAIL)
        return -1;

    setlocale (LC_ALL, ".1251");

    LanguageToken lang_tokens = {};
    LangTokenCtor (&lang_tokens);

    NameTable lang_name_table = {};
    NameTableCtor (&lang_name_table);

    FILE *input_file = fopen (InputFileName (argv), "r");
    assert (input_file);

    LexicalAnalyzer (input_file, &lang_tokens, &lang_name_table);

//    LangTokenDump (&lang_tokens);

    fclose (input_file);
    input_file = NULL;

    /*
    for (size_t i = 0; lang_tokens.data.node_array[i]; i++) {
        NodeTypePrint (stderr, lang_tokens.data.node_array[i], &lang_name_table);
        fprintf (stderr, "|");
        fprintf (stderr, "%s", (lang_tokens.data.char_array)[lang_tokens.data.index_node_word[i]]);
        fprintf (stderr, "|\n");
    }
    fprintf (stderr, "\ngde\n");
    */
    
    Tree lang_tree = {};
    TreeCtor (&lang_tree);

    lang_tree.root = GetGrammar (&lang_tokens);

    if (!lang_tree.root)
        return -1;

    if (SemanticAnalyzer (&lang_tree, &lang_name_table) != 0)
        return -2;

    FILE *output_file = fopen (OutputFileName (argv), "w");
    assert (output_file);

    LangTreeFilePrint (output_file, &lang_tree, &lang_name_table);

    fclose (output_file);
    output_file = NULL;

    TreeDtor (&lang_tree);
    NameTableDtor (&lang_name_table);
    LangTokenDtor (&lang_tokens);

    return 0;
}

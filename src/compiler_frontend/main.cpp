#include <stdio.h>
#include <assert.h>
#include <locale.h>
#include <ctype.h>

#include "lexical_quotations.h"
#include "lexical_analyzer.h"
#include "syntax_analyzer.h"
#include "../../lib/tree/tree_log.h"

int main (const int argc, const char *argv[]) {

    setlocale (LC_ALL, ".1251");

    LanguageToken test_token = {};
    NameTable test_name_table = {};

    NameTableCtor (&test_name_table);

//    FILE *test_file = fopen ("test.txt", "r");

//    LexicalAnalyzer (test_file, &test_token, &test_name_table);

    /*
    for (size_t i = 0; test_token.node_array[i]; i++) {
        NodeTypePrint (stderr, test_token.node_array[i], &test_name_table);
        fprintf (stderr, "|");
        fprintf (stderr, "%s", (test_token.char_array)[test_token.index_node_word[i]]);
        fprintf (stderr, "|\n");
    }
    fprintf (stderr, "\ngde\n");
    */

    Tree test_tree = {};
    TreeCtor (&test_tree);

//    test_tree.root = GetGrammar (&test_token);

//    SemanticAnalyzer (test_tree);

//    MathTreeGraphDump (&test_tree, &test_name_table);

//    LangTreeFilePrint (stdout, &test_tree, &test_name_table);

    FILE *test_file = fopen ("test.txt", "r");

    LangTreeNodeRead (test_file, &test_tree.root, &test_name_table);

    MathTreeGraphDump (&test_tree, &test_name_table);

    TreeDestruct (&test_tree);
    NameTableDtor (&test_name_table);

    return 0;
}
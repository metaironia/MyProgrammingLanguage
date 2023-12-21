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

    FILE *test_file = fopen ("test.txt", "r");

    LexicalAnalyzer (test_file, &test_token, &test_name_table);

    fprintf (stderr, "\ngde\n");

    Tree test_tree = {};
    TreeCtor (&test_tree);

    test_tree.root = GetGrammar (&test_token);

    MathTreeGraphDump (&test_tree, &test_name_table);

    return 0;
}
#include <stdio.h>

#include "lexical_analyzer.h"

int main (const int argc, const char *argv[]) {

    LanguageToken test_token = {};

    FILE *test_file = fopen ("test.txt", "r");

    LexicalAnalyzer (test_file, &test_token);

    for (size_t i = 0; (test_token -> node_array)[i]; i++)
        printf ("%s", );


    return 0;
}
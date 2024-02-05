#include <stdio.h>
#include <assert.h>

#include "../../lib/tree/tree_func.h"
#include "../../lib/tree/tree_log.h"

#include "../../lib/tree/math_tree/dsl_commands.h"
#include "../../lib/tree/math_tree/math_tree_func.h"

#include "backend.h"

int main (const int argc, const char *argv[]) {

    FILE *test_file_input = fopen ("test_in.txt", "r");
    assert (test_file_input);

    FILE *test_file_output = fopen ("test_out.txt", "w");
    assert (test_file_output);

    Tree test_tree = {};
    NameTable test_name_table = {};

    NameTableCtor (&test_name_table);

    LangTreeNodeRead (test_file_input, &(test_tree.root), &test_name_table);

    fclose (test_file_input);

    AsmFileLangOperatorWrite (test_file_output, test_tree.root, &test_name_table);

    fclose (test_file_output);

    TreeDestruct (&test_tree);
    NameTableDtor (&test_name_table);

    return 0;
}

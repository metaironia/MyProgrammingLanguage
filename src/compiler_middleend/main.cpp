#include <stdio.h>
#include <assert.h>

#include "../../lib/oneginlib/functions_for_input.h"
#include "../../lib/oneginlib/functions_for_output.h"

#include "../../lib/tree/tree_func.h"
#include "../../lib/tree/math_tree/math_tree_func.h"

#include "middleend.h"

int main (const int argc, const char *argv[]) {

    if (MiddleendCheckCmdArgs (argc) == MIDDLEEND_STATUS_FAIL)
        return -1;

    Tree lang_tree = {};

    NameTable lang_name_table = {};
    NameTableCtor (&lang_name_table);

    FILE *input_file = fopen (InputFileName (argv), "r");

    LangTreeNodeRead (input_file, &(lang_tree.root), &lang_name_table);
    assert (lang_tree.root);

    fclose (input_file);
    input_file = NULL;

    MathTreeNodeConstantsSimplify (lang_tree.root);

    FILE *output_file = MiddleendOutputFileSet (argc, argv);
    assert (output_file);

    LangTreeFilePrint (output_file, &lang_tree, &lang_name_table);

    fclose (output_file);
    output_file = NULL;

    NameTableDtor (&lang_name_table);
    TreeDtor (&lang_tree);

    return 0;
}

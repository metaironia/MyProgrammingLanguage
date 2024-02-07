#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys\stat.h>

#include "../../lib/oneginlib/functions_for_input.h"
#include "asm_func.h"


int main (const int argc, const char *argv[]) {

//============================ Inputting asm file part =================================

    if (CommandLineArgChecker (argc, argv) == ASM_FUNC_FAIL)
        return -1;

    struct stat asm_file_stat = {};

    char *buf = BufFromFile (FileToAssembleName (argv), &asm_file_stat);

    const size_t number_of_strings = StringCounter (buf, asm_file_stat.st_size);

    PtrToStr *pointers_to_strings = PointersToStringsMaker (number_of_strings);

    assert (pointers_to_strings);

    SetPointersToBuf (pointers_to_strings, buf, asm_file_stat.st_size, number_of_strings);

    SlashRSymbolChecker (pointers_to_strings, number_of_strings);

//========================== ASM file analysis  ==================================================

    double *code_array = (double *) calloc (number_of_strings * 2, sizeof (double));
    size_t position_in_code_array = 0;

    LabelForJump labels_to_jmp[MAX_NUM_OF_LABELS] = {};
    size_t labels_counter = 0;

//-------------------------- First compilation ---------------------------------------------------

    for (size_t current_string = 0; current_string < number_of_strings; current_string++)
        if (Assemble (pointers_to_strings, current_string, code_array,
            &position_in_code_array, labels_to_jmp, &labels_counter, 1) == ASM_FUNC_FAIL)

            return -1;

//-------------------------- Second compilation --------------------------------------------------

    position_in_code_array = 0;

    for (size_t current_string = 0; current_string < number_of_strings; current_string++)
        if (Assemble (pointers_to_strings, current_string, code_array,
            &position_in_code_array, labels_to_jmp, &labels_counter, 2) == ASM_FUNC_FAIL)

            return -1;

//========================== Output byte code  ===================================================

    FILE *byte_code = fopen (FileAfterAssembleName (argv), "wb");
    assert (byte_code);

//-------------------------- Header writing ------------------------------------------------------

    WriteHeaderAsm (byte_code);

//-------------------------- Byte code writing ---------------------------------------------------

    position_in_code_array = 0;

    for (; position_in_code_array < number_of_strings * 2; position_in_code_array++)
        WriteToBinFile (code_array, position_in_code_array, byte_code);

    fclose (byte_code);
    byte_code = NULL;

    return 0;
}

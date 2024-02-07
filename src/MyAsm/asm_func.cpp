#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "../../lib/oneginlib/functions_for_input.h"
#include "asm_func.h"
#include "asm_log.h"
#include "extern_for_asm.h"


#define DEF_CMD(cmd_name, cmd_num, have_arg, ...)                                                                \
                                                                                                                 \
                                if (strcmp (first_word, #cmd_name) == 0) {                                       \
                                                                                                                 \
                                    if (ParseAndSetArgs (cmd_num,                                                \
                                                         ptrs_to_strings[current_str].pointer_to_string,         \
                                                         current_pos_str, code_arr, position_in_code_arr,        \
                                                         have_arg) == ASM_FUNC_FAIL) {                                    \
                                                                                                                 \
                                        PrintError (current_str + 1,                                             \
                                                    ptrs_to_strings[current_str].pointer_to_string);             \
                                        return ASM_FUNC_FAIL;                                                             \
                                    }                                                                            \
                                }                                                                                \
                                else

#define DEF_COND_JMP  DEF_JMP

#define DEF_JMP(cmd_name, cmd_num, have_arg, ...)                                                                     \
                                                                                                            \
                                if (strcmp (first_word, #cmd_name) == 0) {                                  \
                                                                                                            \
                                    if (JumpParse (cmd_num,                                                 \
                                                   ptrs_to_strings[current_str].pointer_to_string,          \
                                                   current_pos_str, code_arr, position_in_code_arr,         \
                                                   labels, *label_counter, num_of_compilation) == ASM_FUNC_FAIL) {   \
                                                                                                            \
                                        PrintError (current_str + 1,                                        \
                                                    ptrs_to_strings[current_str].pointer_to_string);        \
                                        return ASM_FUNC_FAIL;                                                        \
                                    }                                                                       \
                                }                                                                           \
                                else


enum AsmFuncStatus Assemble (PtrToStr *const ptrs_to_strings, const size_t current_str,
                             double *const code_arr,          size_t *const position_in_code_arr,
                             LabelForJump *const labels,      size_t *label_counter,
                             const int num_of_compilation) {

    assert (ptrs_to_strings);
    assert (code_arr);
    assert (position_in_code_arr);

    FindCommentaryInString (ptrs_to_strings, current_str);

    if (IsRestStringEmpty (ptrs_to_strings[current_str].pointer_to_string, 0))
        return ASM_FUNC_OK;

    char first_word[MAX_WORD_LENGTH] = "";

    size_t current_pos_str = 0;

    sscanf (ptrs_to_strings[current_str].pointer_to_string, "%s%n", first_word, &current_pos_str);

    if (first_word[0] == ':') {

        if (num_of_compilation >= 2)
            return ASM_FUNC_OK;

        MyStrncpy (labels[*label_counter].name, first_word + 1, MAX_WORD_LENGTH);
        labels[*label_counter].address = *position_in_code_arr;

        (*label_counter)++;

        if (IsRestStringEmpty (ptrs_to_strings[current_str].pointer_to_string, current_pos_str))
            return ASM_FUNC_OK;
        else
            return ASM_FUNC_FAIL;
    }

    #include "../commands.h"

    /* else */ {

        PrintError (current_str + 1, ptrs_to_strings[current_str].pointer_to_string);

        return ASM_FUNC_FAIL;
    }

    return ASM_FUNC_OK;
}

/*enum AsmFuncStatus RegFind (PtrToStr *ptrs_to_strs, size_t curr_str, int *reg_number) {

    char reg_symbol = 0;
    int scanf_scanned_symbols = 0;

    sscanf (ptrs_to_strs[curr_str].pointer_to_string, "%*s r%cx%n", &reg_symbol, &scanf_scanned_symbols);

    if (scanf_scanned_symbols != 0 && 'a' <= reg_symbol && reg_symbol <= 'd') {

        *reg_number = (reg_symbol - 'a');

        return ASM_FUNC_OK;
    }                      //TODO maybe switch ASM_FUNC_FAIL and ASM_FUNC_OK

    return ASM_FUNC_FAIL;
} */

enum AsmFuncStatus EmitCodeNoArg (double *const arr_of_code,  size_t *const pos, const int command_code) {

    assert (arr_of_code);
    assert (pos);

    arr_of_code[(*pos)++] = command_code;

    return ASM_FUNC_OK;
}

enum AsmFuncStatus EmitCodeArg (double *const arr_of_code,  size_t *const pos,
                                const int command_code,     const double val) {

    assert (arr_of_code);
    assert (pos);

    arr_of_code[(*pos)++] = command_code;
    arr_of_code[(*pos)++] = val;


    return ASM_FUNC_OK;
}

enum AsmFuncStatus EmitCodeReg (double *const arr_of_code,  size_t *const pos,
                                const int command_code,     const int reg) {

    assert (arr_of_code);
    assert (pos);

    arr_of_code[(*pos)++] = command_code;
    arr_of_code[(*pos)++] = reg;

    return ASM_FUNC_OK;
}

enum AsmFuncStatus EmitCodeArgAndReg (double *const arr_of_code,  size_t *const pos,
                                      const int command_code,     const int reg,     const double val) {

    assert (arr_of_code);
    assert (pos);

    arr_of_code[(*pos)++] = command_code;
    arr_of_code[(*pos)++] = val;
    arr_of_code[(*pos)++] = reg;

    return ASM_FUNC_OK;
}

enum AsmFuncStatus WriteToBinFile (double *const arr_of_code, const size_t pos, FILE *bin_to_write) {

    assert (arr_of_code);
    assert (bin_to_write);

    int writes = -1;

    if ((writes = fwrite (arr_of_code + pos, sizeof (arr_of_code[0]), 1, bin_to_write)) == 1)
        LOG_PRINT_ASM (ASM_LOG_FILE, "Successful write: wrote ");

    else
        LOG_PRINT_ASM (ASM_LOG_FILE, "Write was failed: expected to write ");

    LOG_PRINT_ASM (ASM_LOG_FILE, "value %.5lf in code array to position %Iu (", arr_of_code[pos], pos);

    unsigned long long command_code = 0;

    memcpy (&command_code, &arr_of_code[pos], sizeof (double));

    for (int current_byte = sizeof (double) * 8 - 1; current_byte >= 0; current_byte--)
        LOG_PRINT_ASM (ASM_LOG_FILE, "%d", ((((command_code & (1ll << current_byte)) == 0) ? 0 : 1)));

    LOG_PRINT_ASM (ASM_LOG_FILE, ")\n");

    return ASM_FUNC_OK;
}

enum AsmFuncStatus WriteHeaderAsm (FILE *bin_to_write) {

    assert (bin_to_write);

    int sign_write = fwrite (ASM_SIGNATURE, sizeof (char),        4, bin_to_write);

    fseek (bin_to_write, sizeof (double), SEEK_SET);

    int vers_write = fwrite (&ASM_VERSION,  sizeof (ASM_VERSION), 1, bin_to_write);

    fseek (bin_to_write, sizeof (double) * 2, SEEK_SET);

    if (sign_write && vers_write) {

        LOG_PRINT_ASM (ASM_LOG_FILE, "Successful header write: signature %s and version %.2lf\n",
                       ASM_SIGNATURE, ASM_VERSION);

        return ASM_FUNC_OK;
    }

    else {

        LOG_PRINT_ASM (ASM_LOG_FILE, "Header write was failed\n");

        return ASM_FUNC_FAIL;
    }
}

enum AsmFuncStatus PrintError (const size_t line, const char* const string_with_error) {

    assert (string_with_error);

    printf ("Syntax error at line %d: %s\n", line, string_with_error);

    return ASM_FUNC_OK;
}

enum AsmFuncStatus FindCommentaryInString (PtrToStr *const ptr_to_strs, const size_t curr_str) {

    assert (ptr_to_strs);

    char *is_commentary = strchr (ptr_to_strs[curr_str].pointer_to_string, COMMENTARY_MARK);

    if (is_commentary)
        *is_commentary = '\0';

    return ASM_FUNC_OK;
}

enum AsmFuncStatus ParseAndSetArgs (int command_num,              const char* const asm_string,
                                    size_t position_in_string,    double * const array_of_code,
                                    size_t *const position,       const int have_arg) {
    if (have_arg == 0) {

        EmitCodeNoArg (array_of_code, position, command_num);

        return ASM_FUNC_OK;
    }

    bool has_bracket = false;

    if ((has_bracket = IsSquareBracket (asm_string, &position_in_string, '[')))
        command_num |= ARG_FORMAT_RAM;

    if (position_in_string == 0 || IsRestStringEmpty (asm_string, position_in_string))
        return ASM_FUNC_FAIL;

    double value = NAN;
    size_t temp_position_in_string = 0;

    sscanf (asm_string + position_in_string, "%lf%n", &value, &temp_position_in_string);

    position_in_string += temp_position_in_string;

    if (!isnan (value)) {

        command_num |= ARG_FORMAT_IMMED;

        EmitCodeArg (array_of_code, position, command_num, value);
    }

    else {

        char reg_symbol = '\0';

        sscanf (asm_string + position_in_string, " r%cx%n", &reg_symbol, &temp_position_in_string);

        if (reg_symbol != '\0' && tolower (reg_symbol) >= 'a' && tolower (reg_symbol) <= 'd') {

            position_in_string += temp_position_in_string;

            command_num |= ARG_FORMAT_REG;

            int reg_number = reg_symbol - 'a';

            sscanf (asm_string + position_in_string, " + %lf%n", &value, &temp_position_in_string);

            if (isnan (value))
                EmitCodeReg (array_of_code, position, command_num, reg_number);

            else {

                position_in_string += temp_position_in_string;

                command_num |= ARG_FORMAT_IMMED;

                EmitCodeArgAndReg (array_of_code, position, command_num, reg_number, value);
            }
        }
    }

    if (has_bracket) {

        if (IsSquareBracket   (asm_string, &position_in_string, ']') &&
            IsRestStringEmpty (asm_string, position_in_string))

            return ASM_FUNC_OK;
    }

    else if (IsRestStringEmpty (asm_string, position_in_string))
        return ASM_FUNC_OK;

    return ASM_FUNC_FAIL;
}

enum AsmFuncStatus JumpParse (const int command_num,      const char* const asm_string,
                              size_t position_in_string, double * const array_of_code,
                              size_t *const position,     LabelForJump *const label_jmp,
                              const size_t label_count,   const int compilation_num) {

    char current_label_name[MAX_WORD_LENGTH] = "";

    size_t temp_position_in_string = 0;

    sscanf (asm_string + position_in_string, "%s%n", current_label_name, &temp_position_in_string);

    position_in_string += temp_position_in_string;

    for (size_t i = 0; i < label_count; i++)
        if (strcmp (current_label_name, (label_jmp[i].name)) == 0) {

            EmitCodeArg (array_of_code, position, command_num, label_jmp[i].address);

            if (IsRestStringEmpty (asm_string, position_in_string))
                return ASM_FUNC_OK;
        }

    if (compilation_num == 1) {

        *position += 2;

        return ASM_FUNC_OK;
    }

    return ASM_FUNC_FAIL;
}

bool IsRestStringEmpty (const char *const string_to_check, size_t pos_in_string) {

    char current_symbol = *(string_to_check + pos_in_string);

    while (current_symbol != '\0') {

        if (!isspace (current_symbol))
            return false;

        current_symbol = *(string_to_check + (++pos_in_string));
    }

    return true;
}

bool IsSquareBracket (const char *const string_to_check, size_t *pos_in_string,
                      const char bracket_type) {

    char check_bracket = '\0';

    size_t temp_pos_in_string = 0;

    sscanf (string_to_check + *pos_in_string, " %n%c", &temp_pos_in_string, &check_bracket);

    if (check_bracket == bracket_type) {

        (*pos_in_string) += temp_pos_in_string + 1;

        return true;
    }

    return false;
}

enum AsmFuncStatus PrintJumpLabel (const LabelForJump *const label_jmp) {

    printf ("number of label:    \t");
    printf ("name of label:      \t");
    printf ("address of label:   \t\n");

    for (size_t i = 0; i < MAX_NUM_OF_LABELS; i++) {

        printf ("%-*d\t",   MAX_WORD_LENGTH, i);
        printf ("%-*.*s\t", MAX_WORD_LENGTH, MAX_WORD_LENGTH, label_jmp[i].name);

        if (label_jmp[i].name[0] != '\0')
            printf ("%-*d\t",   MAX_WORD_LENGTH, label_jmp[i].address);

        printf ("\n");
    }

    return ASM_FUNC_OK;
}

enum AsmFuncStatus CommandLineArgChecker (const int argcc, const char *argvv[]) {

    assert (argvv);

    if (argcc < 3) {

        fprintf (stderr, "Not enough arguments.");
        return ASM_FUNC_FAIL;
    }

    if (argcc > 3) {

        fprintf (stderr, "Too much arguments.");
        return ASM_FUNC_FAIL;
    }

    return ASM_FUNC_OK;
}

const char *FileToAssembleName (const char *argvv[]) {

    return argvv[1];
}

const char *FileAfterAssembleName (const char *argvv[]) {

    return argvv[2];
}

enum AsmFuncStatus MyStrncpy (char *const destination, char *const source, size_t symbols_count) {

    assert (destination);
    assert (source);

    memcpy (destination, source, symbols_count);  // �� � ��� ��� ������.

    destination[symbols_count - 1] = '\0';

    return ASM_FUNC_OK;
}

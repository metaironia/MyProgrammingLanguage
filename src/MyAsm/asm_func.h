#ifndef ASM_FUNC_H
#define ASM_FUNC_H

#include "../../lib/oneginlib/functions_for_input.h"

#define ASM_SIGNATURE "SKA!"

const double ASM_VERSION = 1.2;

const char COMMENTARY_MARK = ';';

enum AsmFuncStatus {

    ASM_FUNC_OK,
    ASM_FUNC_FAIL
};

const int ARG_FORMAT_IMMED = (1 << 5);
const int ARG_FORMAT_REG   = (1 << 6);
const int ARG_FORMAT_RAM   = (1 << 7);

const int MAX_WORD_LENGTH = 20;
const int MAX_NUM_OF_LABELS = 20;

struct LabelForJump {

    size_t address;
    char name[MAX_WORD_LENGTH];
};

enum AsmFuncStatus Assemble (PtrToStr *const ptrs_to_strings, const size_t current_str,
                             double *const code_arr,          size_t *const position_in_code_arr,
                             LabelForJump *const labels,      size_t *label_counter,
                             const int num_of_compilation);

enum AsmFuncStatus EmitCodeNoArg (double *const arr_of_code,  size_t *const pos,
                                  const int command_code);

enum AsmFuncStatus EmitCodeArg (double *const arr_of_code,  size_t *const pos,
                                const int command_code, const double val);

enum AsmFuncStatus EmitCodeReg (double *const arr_of_code,  size_t *const pos,
                                const int command_code, const int reg);

enum AsmFuncStatus EmitCodeArgAndReg (double *const arr_of_code,  size_t *const pos,
                                      const int command_code,     const int reg,     const double val);

enum AsmFuncStatus FindCommentaryInString (PtrToStr *const ptr_to_strs, const size_t curr_str);

enum AsmFuncStatus WriteHeaderAsm (FILE *bin_to_write);

enum AsmFuncStatus WriteToBinFile (double *const arr_of_code, size_t pos, FILE *bin_to_write);

enum AsmFuncStatus PrintError (const size_t line, const char* const string_with_error);

enum AsmFuncStatus ParseAndSetArgs (int command_num,              const char* const asm_string,
                                    size_t position_in_string,    double * const array_of_code,
                                    size_t *const position,       const int have_arg);

enum AsmFuncStatus JumpParse (const int command_num,      const char* const asm_string,
                              size_t position_in_string, double * const array_of_code,
                              size_t *const position,     LabelForJump *const label_jmp,
                              const size_t label_count,   const int compilation_num);

bool IsRestStringEmpty (const char *const string_to_check, size_t position_in_string);

bool IsSquareBracket (const char *const string_to_check, size_t *position_in_string,
                      const char bracket_type);

enum AsmFuncStatus PrintJumpLabel (const LabelForJump *const label_jmp);

enum AsmFuncStatus CommandLineArgChecker (const int argcc, const char *argvv[]);

const char *FileToAssembleName (const char *argvv[]);

const char *FileAfterAssembleName (const char *argvv[]);

enum AsmFuncStatus MyStrncpy (char *const destination, char *const source, size_t symbols_count);

#endif

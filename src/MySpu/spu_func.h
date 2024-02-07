#ifndef SPU_H
#define SPU_H


#include "../../lib/mystacklib/my_stack_func.h"

#define PUSH(arg)          StackPush (&(main_spu.stk), arg)

#define POP                StackPop (&(main_spu.stk))

#define PUSH_RET           StackPush (&(main_spu.stk_ret_addresses), position_in_code_array + 2)

#define POP_RET            StackPop (&(main_spu.stk_ret_addresses))


#define SPU_SIGNATURE "SKA!"

const double SPU_VERSION = 1.2;

const int SIGNATURE_SIZE_BYTES = 4;
const int VERSION_SIZE_BYTES   = 8;

const int ARG_FORMAT_IMMED = (1 << 5);
const int ARG_FORMAT_REG   = (1 << 6);
const int ARG_FORMAT_RAM   = (1 << 7);

const int BYTE_MASK_FOR_CMD = 0x1F;

const int NUMBER_OF_REGS = 4;
const size_t RAM_SIZE = 900;

enum SpuFuncStatus {

    SPU_FUNC_OK,
    SPU_FUNC_FAIL
};

struct SpuStruct {

    Stack stk;
    Stack stk_ret_addresses;
    double regs[NUMBER_OF_REGS]; // rax, rbx, rcx, rdx
    double RAM[RAM_SIZE];
};

enum SpuFuncStatus RunByteCode (FILE *bin_file);

enum SpuFuncStatus CommandLineArgChecker (const int argcc, const char *argvv[]);

const char *BytecodeFileName (const char *argvv[]);

enum SpuFuncStatus SignAndVersionChecker (FILE *bin_to_check);

double *GetArgument (const double *code_arr, size_t *code_arr_position, const char *const command_name,
                     double *spu_RAM, double *spu_regs);

#endif

#ifndef MIDDLEEND_H
#define MIDDLEEND_H

#include <stdio.h>

enum MiddleendStatus {

    MIDDLEEND_FUNC_STATUS_OK,
    MIDDLEEND_FUNC_STATUS_FAIL
};

MiddleendStatus MiddleendCmdArgsCheck (const int argcc);

FILE *MiddleendOutputFileSet (const int argcc, const char *argvv[]);

#endif

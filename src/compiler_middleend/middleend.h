#ifndef MIDDLEEND_H
#define MIDDLEEND_H

#include <stdio.h>

enum MiddleendStatus {

    MIDDLEEND_STATUS_OK,
    MIDDLEEND_STATUS_FAIL
};

MiddleendStatus MiddleendCheckCmdArgs (const int argcc);

FILE *MiddleendOutputFileSet (const int argcc, const char *argvv[]);

#endif

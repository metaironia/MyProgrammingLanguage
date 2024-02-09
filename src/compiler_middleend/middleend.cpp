#include <stdio.h>
#include <assert.h>

#include "../../lib/oneginlib/functions_for_input.h"
#include "../../lib/oneginlib/functions_for_output.h"

#include "middleend.h"


MiddleendStatus MiddleendCheckCmdArgs (const int argcc) {

    if (argcc < 2) {

        fprintf (stderr, "Not enough args. Program will be terminated.\n");
        return MIDDLEEND_STATUS_FAIL;
    }

    else if (argcc > 3) {

        fprintf (stderr, "Too much args. Program will be terminated.\n");
        return MIDDLEEND_STATUS_FAIL;
    }

    return MIDDLEEND_STATUS_OK;
}

FILE *MiddleendOutputFileSet (const int argcc, const char *argvv[]) {

    assert (argvv);

    if (argcc == 2)
        return fopen (InputFileName (argvv), "w");

    else if (argcc == 3)
        return fopen (OutputFileName (argvv), "w");

    return NULL;
}

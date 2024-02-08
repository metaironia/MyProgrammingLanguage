#include <stdio.h>

#include "frontend_arg_checker.h"

FrontendStatus FrontendCheckCmdArgs (const int argcc) {

    if (argcc < 3) {

        fprintf (stderr, "Not enough args. Program will be terminated.\n");
        return FRONTEND_STATUS_FAIL;
    }

    else if (argcc > 3) {

        fprintf (stderr, "Too much args. Program will be terminated.\n");
        return FRONTEND_STATUS_FAIL;
    }

    return FRONTEND_STATUS_OK;
}

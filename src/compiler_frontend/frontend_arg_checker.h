#ifndef FRONTEND_ARG_CHECKER
#define FRONTEND_ARG_CHECKER

enum FrontendStatus {

    FRONTEND_STATUS_OK,
    FRONTEND_STATUS_FAIL
};

FrontendStatus FrontendCheckCmdArgs (const int argcc);

#endif

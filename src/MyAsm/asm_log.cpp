#include <stdlib.h>

#include "asm_log.h"

FILE *ASM_LOG_FILE = AsmLogFileOpen (ASM_LOG_TXT);

FILE *AsmLogFileOpen (const char *log_file) {

    atexit (AsmLogFileClose);

    return fopen (log_file, "w");
}

void AsmLogFileClose (void) {

    if (ASM_LOG_FILE)
        fclose (ASM_LOG_FILE);

    ASM_LOG_FILE = NULL;
}

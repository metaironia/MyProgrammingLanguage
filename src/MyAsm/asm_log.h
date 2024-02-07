#ifndef ASM_LOG_H
#define ASM_LOG_H

#include <stdio.h>

#define LOG_PRINT_ASM(LOG_FILE_ASM, ...)  do {                                          \
                                                  fprintf(LOG_FILE_ASM, __VA_ARGS__);   \
                                                  fflush (LOG_FILE_ASM);                \
                                                                                        \
                                              } while (0)

#define ASM_LOG_TXT    "asm_log.txt"

FILE *AsmLogFileOpen (const char *log_file);

void AsmLogFileClose (void);


#endif


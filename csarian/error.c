#include <stdio.h>

#include "error.h"

int error(int line, int error_type, char *error_message)
{
    switch (error_type)
    {
        case 0:
            fprintf(stderr, "\nMEM_MALLOC_FAILED Error (%d), at line: %d\n%s\n",error_type,line,error_message);
            break;
        case 1:
            fprintf(stderr, "\nMEM_CALLOC_FAILED Error (%d), at line: %d\n%s\n",error_type,line,error_message);
            break;
        case 2:
            fprintf(stderr, "\nMEM_REALLOC_FAILED Error (%d), at line: %d\n%s\n",error_type,line,error_message);
            break;

        default:
            fprintf(stderr, "\n[!] Unknown error type: %d\n",error_type);
    }
    return 1;
}
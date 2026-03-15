// error.h
#ifndef ERROR_H
#define ERROR_H

// Error types
typedef enum
{
    // Memory errors (MEM):
    MEM_MALLOC_FAILED,      // malloc() = null (code 0)
    MEM_CALLOC_FAILED,      // calloc() = null (code 1)
    MEM_REALLOC_FAILED      // realloc() = null (code 2)
} ErrorType;

int error(int line, int error_type, char *error_message);

#endif
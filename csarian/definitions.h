// definitions.h
#ifndef DEFINITIONS_H
#define DEFINITIONS_H

// Token types
typedef enum
{
    TOKEN_PLUS,
    TOKEN_EQUAL,
    TOKEN_NUMERIC,
    // TOKEN_ASSIGNMENT, This will replace the equal token for assigning variables.
    TOKEN_IDENTIFIER,
    TOKEN_STRING
} TokenType;

// Token struct
typedef struct
{
    TokenType type;
    char *value;
} Token;

#endif
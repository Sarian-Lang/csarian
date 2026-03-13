// definitions.h
#ifndef LEXER_H
#define LEXER_H

// Token Types
typedef enum
{
    TOKEN_PLUS,
    TOKEN_NUMERIC
} TokenType;

// Token
typedef struct
{
    TokenType type;
    char *value;
} Token;

#endif
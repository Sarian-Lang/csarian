// definitions.h
#ifndef DEFINITIONS_H
#define DEFINITIONS_H

// Global vars
typedef struct
{
    char* key;      // Variable name
    void* value;
} GlobalVariable;

// Token types
typedef enum
{
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_ASTERISK,
    TOKEN_SLASH,
    TOKEN_PERCENT,
    TOKEN_INTLITERAL,
    TOKEN_FLOATLITERAL,
    TOKEN_ASSIGNMENT,
    TOKEN_IDENTIFIER,
    TOKEN_STRING,
    TOKEN_HASHTAG,
    TOKEN_LPARENT,
    TOKEN_RPARENT,
    TOKEN_IF,
    TOKEN_WHILE,
    TOKEN_FOR,
    TOKEN_ELSE
} TokenType;

// Precedences
typedef enum
{
    NO_PRECEDENCE = 0,

    PLUS_PRECEDENCE = 10,
    MINUS_PRECEDENCE = 10,

    ASTERISK_PRECEDENCE = 20,
    SLASH_PRECEDENCE = 20,
    PERCENT_PRECEDENCE = 20
} TokenPrecedence;

// Token struct
typedef struct
{
    TokenType type;
    char *value;
    TokenPrecedence precedence;
} Token;

#define IS_BINARY_OPERATOR(t) ((t) == TOKEN_PLUS || (t) == TOKEN_MINUS || (t) == TOKEN_ASTERISK || \
                               (t) == TOKEN_SLASH || (t) == TOKEN_PERCENT)

#define IS_VALID_BINARY_OPERAND(t) ((t) == TOKEN_INTLITERAL || (t) == TOKEN_FLOATLITERAL || (t) == TOKEN_STRING || \
                                    (t) == TOKEN_IDENTIFIER)

#endif
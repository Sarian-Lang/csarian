// lexer.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "definitions.h"
#include "interpreter.h"
#include "error.h"

#define MAX_TOKENS 5000

// Debug feature
void printTokens(Token *tokens, int numTokens) {
    for (int i = 0; i < numTokens; i++) { // numTokens + 1 so we can add the TOKEN_EOF.
        switch (tokens[i].type)
        {
            case TOKEN_PLUS:
                printf("[TOKEN_PLUS] ");
                break;
            case TOKEN_MINUS:
                printf("[TOKEN_MINUS] ");
                break;
            case TOKEN_ASTERISK:
                printf("[TOKEN_ASTERISK] ");
                break;
            case TOKEN_SLASH:
                printf("[TOKEN_SLASH] ");
                break;
            case TOKEN_PERCENT:
                printf("[TOKEN_PERCENT] ");
                break;
            case TOKEN_INTLITERAL:
                printf("[TOKEN_INTLITERAL (%s)] ",tokens[i].value);
                break;
            case TOKEN_FLOATLITERAL:
                printf("[TOKEN_FLOATLITERAL (%s)] ",tokens[i].value);
                break;
            case TOKEN_ASSIGNMENT:
                printf("[TOKEN_ASSIGNMENT] ");
                break;
            case TOKEN_IDENTIFIER:
                printf("[TOKEN_IDENTIFIER (%s)] ",tokens[i].value);
                break;
            case TOKEN_STRING:
                printf("[TOKEN_STRING (%s)] ",tokens[i].value);
                break;
            case TOKEN_HASHTAG:
                printf("[TOKEN_HASHTAG] ");
                break;
            case TOKEN_LPARENT:
                printf("[TOKEN_LPARENT] ");
                break;
            case TOKEN_RPARENT:
                printf("[TOKEN_RPARENT] ");
                break;
            case TOKEN_IF:
                printf("[TOKEN_IF] ");
                break;
            case TOKEN_WHILE:
                printf("[TOKEN_WHILE] ");
                break;
            case TOKEN_FOR:
                printf("[TOKEN_FOR] ");
                break;
            case TOKEN_ELSE:
                printf("[TOKEN_ELSE] ");
                break;

            default:
                printf("[UNKNOWN TOKEN] ");
                break;
        }
    }
    printf("\n");
}

int lexer(char *line, int lineNum)
{
    Token tokens[MAX_TOKENS];
    int current_token = 0;
    size_t line_len = strlen(line);

    for (size_t i = 0; i < line_len + 1; i++) // line_len + 1 so we can add the EOF token.
    {
        char current_char = line[i];

        // Strings
        if (current_char == '"' || current_char == '\'')
        {
            char *string_value = calloc(1, 1);
            if (!string_value)
            {
                error(lineNum, 1, "Failed to calloc() string value.");
            }

            for (size_t j = i + 1; j < line_len; j++) // Iterate over string
            {
                if (line[j] == '"' || line[j] == '\'')
                {
                    // Reached the end of the string, we add the string token with 
                    // the string value.
                    tokens[current_token] = (Token){TOKEN_STRING, strdup(string_value)};
                    current_token++;

                    free(string_value);
                    string_value = NULL;

                    i = j;

                    break;
                }
                else
                {
                    // Continue to add string characters if we don't find the end.
                    char *tmp = realloc(string_value, strlen(string_value) + 2);
                    if (!tmp)
                    {
                        error(lineNum, 2, "Failed to realloc() string value.");
                        return 1;
                    }

                    string_value = tmp;

                    size_t len = strlen(string_value);
                    string_value[len] = line[j];
                    string_value[len+1] = '\0';
                }
            }
        }

        // PLUS operator
        if (current_char == '+')
        {
            tokens[current_token] = (Token){TOKEN_PLUS,NULL,PLUS_PRECEDENCE};
            current_token++;
        }
        // MINUS operator
        if (current_char == '-')
        {
            tokens[current_token] = (Token){TOKEN_MINUS,NULL,MINUS_PRECEDENCE};
            current_token++;
        }
        // PERCENT operator
        if (current_char == '%')
        {
            tokens[current_token] = (Token){TOKEN_PERCENT,NULL,PERCENT_PRECEDENCE};
            current_token++;
        }
        // SLASH operator
        if (current_char == '/')
        {
            tokens[current_token] = (Token){TOKEN_SLASH,NULL,SLASH_PRECEDENCE};
            current_token++;
        }
        // ASTERISK operator
        if (current_char == '*')
        {
            tokens[current_token] = (Token){TOKEN_ASTERISK,NULL,ASTERISK_PRECEDENCE};
            current_token++;
        }

        // ASSIGNMENT operator
        if (current_char == '=')
        {
            if (line[i + 1] != '=' && line[i + 1] != '<' && line[i + 1] != '>') // Make sure it's not a comparison
            {
                tokens[current_token] = (Token){TOKEN_ASSIGNMENT};
                current_token++;
            }
            else
            {
                i++;
            }
        }

        // Parents
        if (current_char == '(')
        {
            tokens[current_token] = (Token){TOKEN_LPARENT};
            current_token++;
        }
        if (current_char == ')')
        {
            tokens[current_token] = (Token){TOKEN_RPARENT};
            current_token++;
        }

        // Identifiers
        if (isalpha(current_char))
        {
            char *identifier = calloc(1, 2);
            if (!identifier)
            {
                error(lineNum, 1, "Failed to calloc() identifier array.");
                return 1;
            }
            identifier[0] = current_char;

            for (size_t j = i + 1; j <= line_len; j++) // Iterate until we find the end of the identifier.
            {
                if (isalpha(line[j])) 
                {
                    char *tmp = realloc(identifier, strlen(identifier) + 2);
                    if (!tmp)
                    {
                        error(lineNum, 2, "Failed to realloc() identifier array.");
                        return 1;
                    }
                    identifier = tmp;

                    size_t len = strlen(identifier);
                    identifier[len] = line[j];
                    identifier[len + 1] = '\0';
                }
                else
                {
                    i = j - 1;

                    tokens[current_token] = (Token){TOKEN_IDENTIFIER, strdup(identifier)};
                    current_token++;

                    free(identifier);
                    identifier = NULL;

                    break;
                }
            }
        }

        // Numeric
        if (isdigit(current_char))
        {
            bool is_float = false;

            char *number = calloc(1, 2);
            if (!number)
            {
                error(lineNum, 1, "Failed to calloc() digit value.");
                return 1;
            }
            number[0] = current_char;

            for (size_t j = i + 1; j <= line_len; j++) // Iterate until we find the end of the number
            {
                if (isdigit(line[j]))
                {
                    char *tmp = realloc(number, strlen(number) + 2);
                    if (!tmp)
                    {
                        error(lineNum, 2, "Failed to realloc() digit value.");
                        return 1;
                    }
                    number = tmp;

                    size_t len = strlen(number);
                    number[len] = line[j];
                    number[len + 1] = '\0';
                }
                else if (line[j] == '.') // Detects decimal number
                {
                    is_float = true;

                    char *tmp = realloc(number, strlen(number) + 2);
                    if (!tmp)
                    {
                        error(lineNum, 2, "Failed to realloc() digit value.");
                        return 1;
                    }
                    number = tmp;

                    size_t len = strlen(number);
                    number[len] = line[j];
                    number[len + 1] = '\0';
                }

                else
                {
                    i = j - 1;

                    if (!is_float)
                    {
                        tokens[current_token] = (Token){TOKEN_INTLITERAL, strdup(number)};
                    }
                    else
                    {
                        tokens[current_token] = (Token){TOKEN_FLOATLITERAL, strdup(number)};
                    }

                    current_token++;
                    break;
                }
            }

            free(number);
            number = NULL;
        }

        // Comments
        if (current_char == '#')
        {
            tokens[current_token] = (Token){TOKEN_HASHTAG};
            current_token++;
            line[i] = '\0'; // Strip everything after the comment
        }
    }

    printTokens(tokens, current_token); // DEBUG

    interpreter(tokens, current_token, lineNum);
}
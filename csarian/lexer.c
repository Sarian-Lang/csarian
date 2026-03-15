// lexer.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "definitions.h"
#include "interpreter.h"
#include "error.h"

#define MAX_TOKENS 5000 // Make this dynamic

// Debug feature
void printTokens(Token *tokens, int numTokens) {
    for (int i = 0; i < numTokens; i++) {
        switch (tokens[i].type)
        {
            case TOKEN_PLUS:
                printf("[TOKEN_PLUS] ");
                break;
            case TOKEN_NUMERIC:
                printf("[TOKEN_NUMERIC (%s)] ",tokens[i].value);
                break;
            case TOKEN_IDENTIFIER:
                printf("[TOKEN_IDENTIFIER] ");
                break;
            case TOKEN_STRING:
                printf("[TOKEN_STRING (%s)] ",tokens[i].value);
                break;
            case TOKEN_EQUAL:
                printf("[TOKEN_EQUAL] ");
                break;

            default:
                printf("[UNKNOWN TOKEN] ");
        }
    }
    printf("\n");
}

int lexer(char *line, int lineNum)
{
    Token tokens[MAX_TOKENS];
    int current_token = 0;
    size_t line_len = strlen(line);

    for (size_t i = 0; i < line_len; i++)
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

                    i = j + 1;

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
            tokens[current_token] = (Token){TOKEN_PLUS};
            current_token++;
        }

        // EQUAL operator
        if (current_char == '=')
        {
            if (line[i + 1] != '=' && line[i + 1] != '<' && line[i + 1] != '>') // Make sure it's not a comparison
            {
                tokens[current_token] = (Token){TOKEN_EQUAL};
                current_token++;
            }
            else
            {
                i++;
            }
        }

        // Numeric
        if (isdigit(current_char))
        {
            bool string_added = false;

            char *number = calloc(1, 2);
            if (!number)
            {
                error(lineNum, 1, "Failed to calloc() digit value.");
                return 1;
            }
            number[0] = current_char;

            size_t j;
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
                else
                {
                    i = j - 1;

                    tokens[current_token] = (Token){TOKEN_NUMERIC, strdup(number)};
                    current_token++;

                    break;
                }
            }

            free(number);
            number = NULL;
        }
    }

    printTokens(tokens, current_token);

    interpreter(tokens, current_token);
}
// lexer.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "definitions.h"

#define MAX_TOKENS 5000

int lexer(char *line)
{
    Token tokens[MAX_TOKENS];
    int current_token = 0;

    for (int i = 0; i < strlen(line); i++)
    {
        char current_char = line[i];

        if (current_char == '+')
        {
            tokens[current_token] = (Token){TOKEN_PLUS};
            current_token++;
        }

        if (isdigit(current_char))
        {
            tokens[current_token] = (Token){TOKEN_NUMERIC};
        }
    }

    printf("%d",tokens[0].type);
}
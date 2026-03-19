// sarian.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "lexer.h"
#include "definitions.h"
#include "error.h"

int sarian(char *code)
{
    int lineNum = 1;

    char *line = calloc(1, 1);
    if (!line)
    {
        error(lineNum, 1, "Failed to calloc() line.");
    }

    // Iterate over the code until we find a new line / code-end character.
    size_t code_len = strlen(code);
    for (size_t i = 0; i <= code_len; i++)
    {
        char current_char = code[i];

        if (current_char == '\n' || current_char == '\0')
        {
            lexer(line,lineNum);

            if (current_char == '\0')
            {
                // Don't increment line size since it's the end of the code, not a new line.
            }
            else if (current_char == '\n')
            {
                lineNum++;
            }

            line[0] = '\0'; // Clear line array for the next line.
        }

        else
        {
            // Add current character to the line.
            char *tmp = realloc(line, strlen(line) + 1);
            if (!tmp)
            {
                error(lineNum, 2, "Failed to realloc() line.");
            }
            line = tmp;

            size_t len = strlen(line);
            line[len] = current_char;
            line[len + 1] = '\0';
        }
    }
}
// sarian.c
#include <stdio.h>
#include <string.h>

#include "lexer.h"
#include "definitions.h"

int sarian(char *code)
{
    int lineNum = 1;
    char line[8096]; // Make the line size dynamic

    size_t code_len = strlen(code);
    for (int i = 0; i <= code_len; i++)
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

            line[0] = '\0';
        }

        else
        {
            // Add current character to the line.
            size_t len = strlen(line);
            if (len < sizeof(line) - 1) {
                line[len] = current_char;
                line[len + 1] = '\0';
            }
        }
    }
}
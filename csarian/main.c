// main.c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "sarian.h"
#include "error.h"

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        // Read file
        FILE *file = fopen(argv[1], "r");
        if (!file)
        {
            perror("[Main] Error opening file");
            return 1;
        }

        fseek(file, 0, SEEK_END);
        long filesize = ftell(file);
        fseek(file, 0, SEEK_SET);

        // Read content
        char *code = malloc(filesize + 1);
        if (!code)
        {
            fprintf(stderr, "[Main] Error: insufficient memory\n");
            fclose(file);
            return 1;
        }

        fread(code, 1, filesize, file);
        code[filesize] = '\0';
        fclose(file);

        sarian(code);
        
        free(code);
        code = NULL;
    }
    else
    {
        // REPL
        while (true)
        {
            char input[4028];

            printf("->> ");

            fgets(input, 4028, stdin);

            sarian(input);
            input[0] = '\0';
        }
    }

    return 0;
}
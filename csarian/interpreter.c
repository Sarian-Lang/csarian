// interpreter.c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "definitions.h"
#include "math.h"
#include "error.h"
#include "binaryops.h"

#define CURRENT_TOKEN tokens[i]
#define NEXT_TOKEN tokens[i + 1]
#define NEXT_TOKEN_2 tokens[i + 2]
#define NEXT_TOKEN_3 tokens[i + 3]
#define PREVIOUS_TOKEN tokens[i - 1]

GlobalVariable *global_variables = NULL;
int global_variable_count = 0;

void init_global_variables()
{
    global_variables = calloc(1, sizeof(GlobalVariable));
    if (!global_variables)
    {
        error(0, MEM_CALLOC_FAILED, "Failed to calloc() global variables.");
    }
}

void add_global_variable(char *key, void *value)
{
    GlobalVariable *tmp = realloc(global_variables, sizeof(GlobalVariable) * (global_variable_count + 1));
    if (!tmp)
    {
        error(0, MEM_REALLOC_FAILED, "Failed to realloc() global variables.");
    }

    global_variables = tmp;

    // Add new variable
    global_variables[global_variable_count].key = key; // -1 since global var count already gets added one before this.
    global_variables[global_variable_count].value = value;

    global_variable_count++;
}

bool find_global_variable(char* key)
{
    for (size_t i = 0; i < global_variable_count; i++)
    {
        if (strcmp(global_variables[i].key, key) == 0)
        {
            return true;
        }
    }

    return false;
}

double make_binary_operation(Token *tokens, int numTokens, int lineNum, int i)
{
    // Parse tokens, get only the operation tokens and pass it to binaryops.c

    Token *binary_operation_array = calloc(1, sizeof(Token));
    if (!binary_operation_array)
    {
        error(lineNum, MEM_CALLOC_FAILED, "Failed to calloc() binary operation array.");
    }

    size_t token_count = 0;

    // Checks if the previous character is not a valid operand.
    if (!IS_VALID_BINARY_OPERAND(PREVIOUS_TOKEN.type))
    {
        error(lineNum, SYNTAX_INCOMPLETE_EXPRESSION, "Incomplete binary operation.");
    }

    // Iterate over binary operation tokens to pass the operation
    // tokens to binaryops.c
    for (size_t j = i - 1; j < numTokens; j++)
    {
        if (IS_BINARY_OPERATOR(tokens[j].type))
        {
            binary_operation_array[token_count] = tokens[j];
            token_count++;

            Token *tmp = realloc(binary_operation_array, (token_count + 1) * sizeof(Token));
            if (!tmp)
            {
                error(lineNum, MEM_REALLOC_FAILED, "Failed to realloc() binary operation array.");
            }
            binary_operation_array = tmp;
        }
        else if (IS_VALID_BINARY_OPERAND(tokens[j].type))
        {
            binary_operation_array[token_count] = tokens[j];
            token_count++;
            
            Token *tmp = realloc(binary_operation_array, (token_count + 1) * sizeof(Token));
            if (!tmp)
            {
                error(lineNum, MEM_REALLOC_FAILED, "Failed to realloc() binary operation array.");
            }
            binary_operation_array = tmp;
        }
        else
        {
            i = j + 1;
            break;
        }
        
        i = j + 1;
    }

    if (IS_VALID_BINARY_OPERAND(binary_operation_array[token_count - 1].type))
    {
        void* result_ptr = binary_operation(binary_operation_array, token_count);
        double result = *(double*)result_ptr;

        free(result_ptr);
        result_ptr = NULL;

        return result;
    }
    else
    {
        error(lineNum, SYNTAX_INCOMPLETE_EXPRESSION, "Incomplete binary operation.");
    }
}

int interpreter(Token *tokens, int numTokens, int lineNum)
{
    // Iterate tokens received from lexer.
    size_t i;
    for (i = 0; i < numTokens; i++)
    {
        switch (CURRENT_TOKEN.type)
        {
            // --- Binary operators ---
            case TOKEN_PLUS:
            case TOKEN_MINUS:
            case TOKEN_ASTERISK:
            case TOKEN_SLASH:
            case TOKEN_PERCENT:
                make_binary_operation(tokens, numTokens, lineNum, i);
                break;
            
            case TOKEN_IDENTIFIER:
                // (If identifier is a keyword...)

                // Doesn't match with keywords, we suppose it's a variable 
                // name or function name

                if (find_global_variable(CURRENT_TOKEN.value))
                {
                    break;
                }

                if (NEXT_TOKEN.type == TOKEN_ASSIGNMENT)
                {
                    if (!IS_BINARY_OPERATOR(NEXT_TOKEN_3.type))
                    {
                        add_global_variable(CURRENT_TOKEN.value, NEXT_TOKEN_2.value);
                    }
                    else
                    {
                        // I will continue this next day
                    }
                }

                break;

            default:
                break;
        }
    }
}
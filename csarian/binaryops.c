// binaryops.c
#include <stdio.h>
#include <stdlib.h>

#include "definitions.h"
#include "error.h"
#include "lexer.h"

#define NUMBER_A tokens[i - 1]
#define NUMBER_B tokens[i + 1]
#define NUMBER_C tokens[i + 3]
#define OPERATION tokens[i]
#define NEXT_OPERATION tokens[i + 2]

double calculate(char*a, char*b, TokenType operation_type)
{
    double result = 0;

    switch (operation_type)
    {
        case TOKEN_PLUS:
            result = (atof(a)) + (atof(b));
            break;
        case TOKEN_MINUS:
            result = (atof(a)) - (atof(b));
            break;
        case TOKEN_ASTERISK:
            result = (atof(a)) * (atof(b));
            break;
        case TOKEN_SLASH:
            result = (atof(a)) / (atof(b));
            break;
        case TOKEN_PERCENT:
            result = (atoi(a)) % (atoi(b)); // % Can't use doubles, we use atoi instead.
            break;

        default:
            fprintf(stderr,"\n[! BINARYOPS.C] Unknown operation in NEXT_OPERATION.type switch.\n");
            exit(1);
    }

    return result;
}

void* binary_operation(Token *tokens, int numTokens)
{
    double result = 0.0;

    for (int i = 0; i < numTokens; i++)
    {
        if (numTokens == 1)
        {
            double *result_ptr = malloc(sizeof(double));
            *result_ptr = result;
            return result_ptr;
        }
        
        if (IS_VALID_BINARY_OPERAND(NUMBER_A.type) && IS_VALID_BINARY_OPERAND(NUMBER_B.type))
        {
            if (IS_BINARY_OPERATOR(NEXT_OPERATION.type) && NEXT_OPERATION.precedence > OPERATION.precedence)
            {
                // Precedence operation found ahead.

                result = calculate(NUMBER_B.value, NUMBER_C.value, NEXT_OPERATION.type);

                // Make NUMBER_B the result of C + B
                NUMBER_B.type = TOKEN_INTLITERAL;
                sprintf(NUMBER_B.value, "%g", result);

                // Shift remaining tokens to the left
                for (int j = i + 4; j < numTokens; j++)
                {
                    tokens[j - 3] = tokens[j];
                }

                numTokens -= 1;
                i = -1; // (-1 since the loop will add 1 after we finish this iteration.)
            }
            else
            {
                // No precedence operation found ahead.
                
                        
                result = calculate(NUMBER_A.value, NUMBER_B.value, OPERATION.type);
                        
                // Make NUMBER_A the result of A + B
                tokens[i - 1].type = TOKEN_INTLITERAL;
                sprintf(tokens[i - 1].value, "%g", result);

                // Shift remaining tokens to the left
                for (int j = i + 2; j < numTokens; j++)
                {
                    tokens[j - 2] = tokens[j];
                }

                numTokens -= 2;
                i = -1; // (-1 since the loop will add 1 after we finish this iteration.)
            }
        }
    }

    double *result_ptr = malloc(sizeof(double));
    *result_ptr = result;
    return result_ptr;
}
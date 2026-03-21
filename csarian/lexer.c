// lexer.c
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "definitions.h"
#include "error.h"

int current_token;
size_t tokens_len;
Token *tokens;

void InitTokens()
{
  tokens_len = 32;
  tokens = calloc(tokens_len, sizeof(Token));
  current_token = 0;
}

void AddToken(TokenType token_type, char *token_value, TokenPrecedence token_precedence)
{ 
  if (current_token + 1 >= tokens_len)
  {
    size_t new_len = tokens_len * 2;
    Token *tmp = realloc(tokens, sizeof(Token) * new_len);
    tokens = tmp;

    tokens_len = new_len;
  }

  tokens[current_token] = (Token){token_type, token_value, token_precedence};
  current_token++;
}

// Debug feature
void PrintTokens(Token *tokens, int numTokens) {
  for (int i = 0; i < numTokens; i++) {
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
      case TOKEN_INT_LITERAL:
        printf("[TOKEN_INT_LITERAL (%s)] ", tokens[i].value);
        break;
      case TOKEN_FLOAT_LITERAL:
        printf("[TOKEN_FLOAT_LITERAL (%s)] ", tokens[i].value);
        break;
      case TOKEN_ASSIGNMENT:
        printf("[TOKEN_ASSIGNMENT] ");
        break;
      case TOKEN_IDENTIFIER:
        printf("[TOKEN_IDENTIFIER (%s)] ", tokens[i].value);
        break;
      case TOKEN_STRING:
        printf("[TOKEN_STRING (%s)] ", tokens[i].value);
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
      case TOKEN_LBRACKET:
        printf("[TOKEN_LBRACKET] ");
        break;
      case TOKEN_RBRACKET:
        printf("[TOKEN_RBRACKET] ");
        break;
      case TOKEN_EQUAL:
        printf("[TOKEN_EQUAL] ");
        break;
      case TOKEN_NOT_EQUAL:
        printf("[TOKEN_NOT_EQUAL] ");
        break;
      case TOKEN_LESS:
        printf("[TOKEN_LESS] ");
        break;
      case TOKEN_LESS_EQUAL:
        printf("[TOKEN_LESS_EQUAL] ");
        break;
      case TOKEN_GREATER:
        printf("[TOKEN_GREATER] ");
        break;
      case TOKEN_GREATER_EQUAL:
        printf("[TOKEN_GREATER_EQUAL] ");
        break;
      case TOKEN_AND:
        printf("[TOKEN_AND] ");
        break;
      case TOKEN_OR:
        printf("[TOKEN_OR] ");
        break;
      case TOKEN_EXCLAMATION:
        printf("[TOKEN_EXCLAMATION] ");
        break;
      case TOKEN_EOF:
        printf("[TOKEN_EOF] ");
        break;
      case TOKEN_EOL:
        printf("[TOKEN_EOL] ");
        break;
      case TOKEN_COLON:
        printf("[TOKEN_COLON] ");
        break;

      default:
        printf("[UNKNOWN TOKEN] ");
        break;
    }
  }
  printf("\n");
}

void Lexer(char *code)
{
  int current_line = 1;

  InitTokens();

  size_t code_len = strlen(code);
  for (size_t i = 0; i < code_len + 1; i++)
  {
    char current_char = code[i];
    char next_char = code[i + 1];

    if (i == code_len) // We reached the end of the code.
    {
      AddToken(TOKEN_EOF, NULL, NO_PRECEDENCE);
      break;
    }

    if (current_char == '\n')
    {
      AddToken(TOKEN_EOL, NULL, NO_PRECEDENCE);
    }

    // Comments
    if (current_char == '/')
    {
      if (next_char == '/')
      {
        for (size_t j = i; j < code_len; j++)
        {
          i++;

          if (code[j] == '\n' || code[j] == '\0')
          {
            break;
          }
        }

        i -= 2; // -2 so we can add the EOL/EOF token.
        continue;
      }
    }

    // Strings
    if (current_char == '"' || current_char == '\'')
    {
      size_t string_len = 16;

      char *string_value = calloc(string_len, sizeof(char));
      if (!string_value)
      {
        error(current_line, MEM_CALLOC_FAILED, "Failed to calloc() string value.");
      }

      size_t pos = 0;

      for (size_t j = i + 1; j < code_len; j++)
      {
        if (code[j] == '"' || code[j] == '\'')
        {
          // Reached the end of the string.
          AddToken(TOKEN_STRING, strdup(string_value), NO_PRECEDENCE);

          free(string_value);
          string_value = NULL;

          i = j;

          break;
        }
        else
        {
          // Continue to add string characters if we don't find the end.
          if (pos + 1 >= string_len)
          {
            size_t new_len = string_len *= 2;

            char *tmp = realloc(string_value, new_len);
            if (!tmp)
            {
              error(current_line, MEM_REALLOC_FAILED, "Failed to realloc() string value.");
            }

            string_value = tmp;

            string_len = new_len;
          }

          string_value[pos++] = code[j];
          string_value[pos] = '\0';
        }
      }
    }

    // Identifiers / Keywords
    if (isalpha(current_char))
    {
      size_t identifier_len = 16;

      char *identifier = calloc(identifier_len, sizeof(char));
      if (!identifier)
      {
        error(current_line, MEM_CALLOC_FAILED, "Failed to calloc() identifier array.");
      }

      identifier[0] = current_char;

      size_t pos = 1;

      for (size_t j = i + 1; j <= code_len; j++) // Iterate until we find the end of the identifier.
      {
        if (isalpha(code[j]) || isdigit(code[j]))
        {
          if (pos + 1 >= identifier_len)
          {
            size_t new_len = identifier_len * 2;

            char *tmp = realloc(identifier, new_len);
            if (!tmp)
            {
              error(current_line, MEM_REALLOC_FAILED, "Failed to realloc() identifier array.");
            }

            identifier = tmp;
            identifier_len *= 2;
          }

          identifier[pos++] = code[j];
          identifier[pos + 1] = '\0';
        }
        else
        {
          i = j - 1;

          // Keywords
          if (strcmp(identifier, "and") == 0)
          {
            AddToken(TOKEN_AND, NULL, NO_PRECEDENCE);
          }
          else if (strcmp(identifier, "or") == 0)
          {
            AddToken(TOKEN_OR, NULL, NO_PRECEDENCE);
          }
          else if (strcmp(identifier, "if") == 0)
          {
            AddToken(TOKEN_IF, NULL, NO_PRECEDENCE);
          }
          else if (strcmp(identifier, "else") == 0)
          {
            AddToken(TOKEN_ELSE, NULL, NO_PRECEDENCE);
          }
          else if (strcmp(identifier, "while") == 0)
          {
            AddToken(TOKEN_ELSE, NULL, NO_PRECEDENCE);
          }
          else if (strcmp(identifier, "for") == 0)
          {
            AddToken(TOKEN_ELSE, NULL, NO_PRECEDENCE);
          }
          // Not a keyword, adding normal identifier token.
          else 
          {
            AddToken(TOKEN_IDENTIFIER, strdup(identifier), NO_PRECEDENCE);
          }

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
      size_t number_len = 16;

      char *number = calloc(number_len, sizeof(char));
      if (!number)
      {
        error(current_line, MEM_CALLOC_FAILED, "Failed to calloc() digit value.");
      }

      number[0] = current_char;

      size_t pos = 1;

      for (size_t j = i + 1; j <= code_len; j++) // Iterate until we find the end of the number
      {
        if (isdigit(code[j]))
        {
          if (pos + 1 >= number_len)
          {
            size_t new_len = number_len * 2;

            char *tmp = realloc(number, new_len);
            if (!tmp)
            {
              error(current_line, MEM_REALLOC_FAILED, "Failed to realloc() digit value.");
            }

            number = tmp;
            number_len *= 2;
          }

          number[pos++] = code[j];
          number[pos + 1] = '\0';
        }
        else if (code[j] == '.') // Detects decimal number
        {
          size_t new_len = number_len * 2;
          is_float = true;

          if (pos + 1 >= number_len)
          {
            char *tmp = realloc(number, new_len);
            if (!tmp)
            {
              error(current_line, MEM_REALLOC_FAILED, "Failed to realloc() digit value.");
            }

            number = tmp;
            number_len *= 2;
          }

          number[pos++] = code[j];
          number[pos + 1] = '\0';
        }
        else
        {
          i = j - 1;

          if (!is_float)
          {
            AddToken(TOKEN_INT_LITERAL, strdup(number), NO_PRECEDENCE);
          }
          else
          {
            AddToken(TOKEN_FLOAT_LITERAL, strdup(number), NO_PRECEDENCE);
          }

          break;
        }
      }

      free(number);
      number = NULL;
    }

    // COLON
    if (current_char == ':')
    {
      AddToken(TOKEN_COLON, NULL, NO_PRECEDENCE);
    }

    // PLUS operator
    if (current_char == '+')
    {
      AddToken(TOKEN_PLUS, NULL, PLUS_PRECEDENCE);
    }
    // MINUS operator
    if (current_char == '-')
    {
      AddToken(TOKEN_MINUS, NULL, MINUS_PRECEDENCE);
    }
    // PERCENT operator
    if (current_char == '%')
    {
      AddToken(TOKEN_PERCENT, NULL, PERCENT_PRECEDENCE);
    }
    // SLASH operator
    if (current_char == '/')
    {
      AddToken(TOKEN_SLASH, NULL, SLASH_PRECEDENCE);
    }
    // ASTERISK operator
    if (current_char == '*')
    {
      AddToken(TOKEN_ASTERISK, NULL, ASTERISK_PRECEDENCE);
    }
    // ASSIGNMENT / EQUAL operator
    if (current_char == '=')
    {
      if (next_char != '=')
      {
        AddToken(TOKEN_ASSIGNMENT, NULL, NO_PRECEDENCE);
      }
      else
      {
        AddToken(TOKEN_EQUAL, NULL, NO_PRECEDENCE);
        i++;
      }
    }
    // GREATER / GREATER OR EQUAL operator
    if (current_char == '>')
    {
      if (next_char != '=')
      {
        AddToken(TOKEN_GREATER, NULL, NO_PRECEDENCE);
      }
      else 
      {
        AddToken(TOKEN_GREATER_EQUAL, NULL, NO_PRECEDENCE);
        i++;
      }
    }
    // LESS / LESS OR EQUAL operator
    if (current_char == '<')
    {
      if (next_char != '=')
      {
        AddToken(TOKEN_LESS, NULL, NO_PRECEDENCE);
      }
      else 
      {
        AddToken(TOKEN_LESS_EQUAL, NULL, NO_PRECEDENCE);
        i++;
      }
    }
    // EXCLAMATION MARK / NOT EQUAL operator
    if (current_char == '!')
    {
      if (next_char != '=')
      {
        AddToken(TOKEN_EXCLAMATION, NULL, NO_PRECEDENCE);
      }
      else 
      {
        AddToken(TOKEN_NOT_EQUAL, NULL, NO_PRECEDENCE);
        i++;
      }
    }

    // Parents
    if (current_char == '(')
    {
      AddToken(TOKEN_LPARENT, NULL, NO_PRECEDENCE);
    }
    if (current_char == ')')
    {
      AddToken(TOKEN_RPARENT, NULL, NO_PRECEDENCE);
    }

    // Brackets
    if (current_char == '{')
    {
      AddToken(TOKEN_LBRACKET, NULL, NO_PRECEDENCE);
    }
    if (current_char == '}')
    {
      AddToken(TOKEN_RBRACKET, NULL, NO_PRECEDENCE);
    }
  }

  PrintTokens(tokens, current_token); // DEBUG

  //interpreter(tokens, current_token, lineNum); OLD
}
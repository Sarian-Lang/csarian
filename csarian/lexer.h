// lexer.h
#ifndef LEXER_H
#define LEXER_H

#include "definitions.h"

int Lexer(char *code);

void PrintTokens(Token *tokens, int numTokens);

#endif
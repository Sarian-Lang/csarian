// lexer.h
#ifndef LEXER_H
#define LEXER_H

#include "definitions.h"

int lexer(char *line, int lineNum);

void printTokens(Token *tokens, int numTokens);

#endif
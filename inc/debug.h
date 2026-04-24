#pragma once
#include "ir.h"
#include "lexer.h"
#include "parser.h"

void debug_lexer(TokenList *tokens);
void debug_parser(AST *ast);
void debug_ir(IRProg *ir);

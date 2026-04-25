#pragma once

#include "translation/ir.h"
#include "init/lexer.h"
#include "init/parser.h"

void debug_lexer(TokenList *tokens);
void debug_parser(AST *ast);
void debug_ir(IRProg *ir);

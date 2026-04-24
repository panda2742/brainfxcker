#include "ir.h"
#include "lexer.h"
#include "optimizer.h"
#include "parser.h"
#include "debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int verify_architecture() {
#if defined(__x86_64__) || defined(_M_X64)
    printf("Compiled for x86-64 architecture\n");
    return 1;
#elif defined(__i386__) || defined(_M_IX86)
    printf("Compiled for x86 (32-bit) - NOT EXPECTED\n");
    return 0;
#else
    printf("? Unknown architecture\n");
    return 0;
#endif
}

int main(int argc, char *argv[]) {
    if (!verify_architecture()) return EXIT_FAILURE;

    if (argc ^ 2)
    {
        fprintf(stderr, "Usage: ./brainfxcker <program.bf>\n");
        exit(EXIT_FAILURE);
    }

    TokenList   lexed = lex_file(argv[1]);
    AST         ast;
    parse_block(lexed.data, lexed.count, &ast.nodes, &ast.count);
    IRProg      prog = {0};
    debug_lexer(&lexed);
    debug_parser(&ast);
    gen_ir_nodes(&prog, ast.nodes, ast.count);
    debug_ir(&prog);
    pass_contract(&prog);
    debug_ir(&prog);

    return EXIT_SUCCESS;
}

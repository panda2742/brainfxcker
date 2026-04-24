#include "codegen.h"
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
    // if (!verify_architecture()) return EXIT_FAILURE;

    if (argc ^ 2)
    {
        fprintf(stderr, "Usage: ./brainfxcker <program.bf>\n");
        exit(EXIT_FAILURE);
    }

    TokenList   lexed = lex_file(argv[1]);
    AST         ast;
    parse_block(lexed.data, lexed.count, &ast.nodes, &ast.count);
    IRProg      prog = {0};
    gen_ir_nodes(&prog, ast.nodes, ast.count);
    pass_contract(&prog);
    debug_ir(&prog);

    char    *output_path;
    FILE    *out = create_output(argv[1], &output_path);
    if (!out) return EXIT_FAILURE;

    write_instructions(out, &prog);
    fclose(out);

    char    big_mama[65536];
    sprintf(big_mama, "nasm -f elf64 %s\n", output_path);
    int res = system(big_mama);
    output_path[strlen(output_path) - 1] = 'o';
    sprintf(big_mama, "ld %s\n", output_path);
    res = system(big_mama);
    (void)res;

    return EXIT_SUCCESS;
}

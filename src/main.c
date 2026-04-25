#include "translation/codegen.h"
#include "translation/ir.h"
#include "init/lexer.h"
#include "translation/optimizer.h"
#include "translation/output.h"
#include "init/parser.h"
#include <stddef.h>
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

int	main(int argc, char *argv[]) {
	if (!verify_architecture()) return EXIT_FAILURE;

	if (argc ^ 2)
	{
		fprintf(stderr, "Usage: ./brainfxcker <program.bf>\n");
		exit(EXIT_FAILURE);
	}

	TokenList	lexed = lex_file(argv[1]);
	AST			ast;
	parse_block(lexed.data, lexed.count, &ast.nodes, &ast.count);
	IRProg		prog = {0};
	gen_ir_nodes(&prog, ast.nodes, ast.count);
	pass_contract(&prog);

	Output	out = create_output(argv[1]);
	write_instructions(out.asm.file, &prog);
	fclose(out.asm.file);
	printf("%s && %s\n", out.asm.cmd, out.object.cmd);
	int	_ = system(out.asm.cmd);
	_ = system(out.object.cmd);
	(void)_;
	return EXIT_SUCCESS;
}

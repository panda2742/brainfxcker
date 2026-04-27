#include "translation/codegen.h"
#include "translation/ir.h"
#include "init/lexer.h"
#include "translation/optimizer.h"
#include "translation/output.h"
#include "init/parser.h"
#include "utils/paged_vector.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int verify_architecture() {
#if defined(__x86_64__) || defined(_M_X64)
	// printf("Compiled for x86-64 architecture\n");
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

	PagedVector	*lexed = lex_file(argv[1]);
	AST			ast;
	parse_block(lexed, 0, lexed->count, &ast.nodes, &ast.count);
	PagedVector	*prog = pv_create(sizeof(IRInstr));
	gen_ir_nodes(prog, ast.nodes, ast.count);
	prog = pass_contract(prog);

	Output	out = create_output(argv[1]);
	write_instructions(out.asm.file, prog);
	fclose(out.asm.file);
	int	_ = system(out.asm.cmd);
	_ = system(out.object.cmd);
	remove(out.asm.path);
	remove(out.object.path);
	pv_free(lexed);
	pv_free(prog);
	free_output(&out);
	free_nodes(ast.nodes, ast.count);
	(void)_;
	return EXIT_SUCCESS;
}

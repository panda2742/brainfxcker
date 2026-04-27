#include "translation/codegen.h"
#include "translation/ir.h"
#include "utils/paged_vector.h"
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static size_t	loop_stack[1024];
static size_t	loop_stack_top = 0;
static size_t	loop_counter = 0;

static int	write_header_(FILE *out)
{
	return fprintf(out,
		"bits 64\n"
		"section .bss\n"
		"  tape resb 30000\n\n"
		"section .text\n"
		"global _start\n"
		"_start:\n"
		"  lea rbx, [rel tape]\n"
	);
}

static int	write_footer_(FILE *out)
{
	return fprintf(out,
		"  mov rax, 60\n"
		"  xor rdi, rdi\n"
		"  syscall\n"
	);
}

int write_instructions(FILE *out, PagedVector *prog)
{
	int res = 0, tmp = 0;

	if ((tmp += write_header_(out)) <= res)
	{
		perror("write_header");
		exit(EXIT_FAILURE);
	}

	for (size_t i = 0; i < prog->count; ++i)
	{
		IRInstr *instr = pv_get(prog, i);
		switch (instr->op)
		{
			case IR_ADD:
				tmp = fprintf(out, "  add byte [rbx], %d\n", instr->arg);
			break;
			case IR_MOVE:
				tmp = fprintf(out, "  add rbx, %d\n", instr->arg);
			break;
			case IR_SET:
				tmp = fprintf(out, "  mov byte [rbx], %d\n", instr->arg);
			break;
			case IR_OUTPUT:
				tmp = fprintf(out,
					"  mov rax, 1\n"
					"  mov rdi, 1\n"
					"  mov rsi, rbx\n"
					"  mov rdx, 1\n"
					"  syscall\n"
				);
			break;
			case IR_INPUT:
				tmp = fprintf(out,
					"  mov rax, 0\n"
					"  mov rdi, 0\n"
					"  mov rsi, rbx\n"
					"  mov rdx, 1\n"
					"  syscall\n"
				);
			break;case IR_LOOP_BEG:
			{
				size_t id = loop_counter++;
				loop_stack[loop_stack_top++] = id;
				tmp = fprintf(out,
					".L%zu_beg:\n"
					"  cmp byte [rbx], 0\n"
					"  je .L%zu_end\n",
					id, id
				);
				break;
			}
			case IR_LOOP_END:
			{
				size_t id = loop_stack[--loop_stack_top];
				tmp = fprintf(out,
					"  cmp byte [rbx], 0\n"
					"  jne .L%zu_beg\n"
					".L%zu_end:\n",
					id, id
				);
				break;
			}
			default: break;
		}
		if (tmp <= res)
		{
			perror("write_instr");
			exit(EXIT_FAILURE);
		}
	}

	if ((tmp += write_footer_(out)) <= res)
	{
		perror("write_header");
		exit(EXIT_FAILURE);
	}
	return res;
}

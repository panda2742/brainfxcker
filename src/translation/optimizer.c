#include "translation/optimizer.h"
#include "translation/ir.h"
#include "utils/paged_vector.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static void pass_remove_nop_(PagedVector *prog)
{
	for (size_t i = 0; i < prog->count; ++i)
	{
		if (((IRInstr *)(pv_get(prog, i)))->op != IR_NOP) continue;
		memmove(((IRInstr *)(pv_get(prog, i))), ((IRInstr *)(pv_get(prog, i + 1))), prog->count - i);
		memmove(
			pv_get(prog, i),
			pv_get(prog, i + 1),
			(prog->count - i - 1) * prog->sib
		);
		--prog->count;
	}
}

static void pass_clean_loop_(PagedVector *prog)
{
	for (size_t i = 0; i + 2 < prog->count; ++i)
	{
		IRInstr *a = ((IRInstr *)(pv_get(prog, i)));
		IRInstr *b = ((IRInstr *)(pv_get(prog, i + 1)));
		IRInstr *c = ((IRInstr *)(pv_get(prog, i + 2)));

		if (a->op == IR_LOOP_BEG && b->op == IR_ADD && (b->arg == 1 || b->arg == -1) && c->op == IR_LOOP_END)
		{
			a->op = 0;
			b->op = c->op = IR_NOP;
		}
	}
	pass_remove_nop_(prog);
}

void	pass_contract(PagedVector *prog)
{
	PagedVector	*out = pv_create(prog->sib);

	for (size_t i = 0; i < prog->count; )
	{
		IRInstr *cur = ((IRInstr *)(pv_get(prog, i)));

		if (cur->op == IR_ADD || cur->op == IR_MOVE)
		{
			int sum = cur->arg;
			size_t	j = i + 1;
			while (j < prog->count && ((IRInstr *)(pv_get(prog, j)))->op == cur->op)
			{
				sum += ((IRInstr *)(pv_get(prog, j)))->arg;
				++j;
			}
			if (sum != 0)
				pv_push(out, &(IRInstr){cur->op, sum, 0});
			i = j;
		}
		else
		{
			pv_push(out, cur);
			++i;
		}
	}
	pass_clean_loop_(out);
	prog->count = out->count;
	prog->page_count = out->page_count;
	prog->pages = out->pages;
	prog->sib = out->sib;
}

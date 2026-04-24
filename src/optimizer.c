#include "optimizer.h"
#include "ir.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static void pass_remove_nop_(IRProg *prog)
{
    for (size_t i = 0; i < prog->count; ++i)
    {
        if (prog->instrs[i].op != IR_NOP) continue;
        memmove(&prog->instrs[i], &prog->instrs[i + 1], prog->count - i);
    }
}

static void pass_clean_loop_(IRProg *prog)
{
    for (size_t i = 0; i + 2 < prog->count; ++i)
    {
        IRInstr *a = &prog->instrs[i];
        IRInstr *b = &prog->instrs[i + 1];
        IRInstr *c = &prog->instrs[i + 2];

        if (a->op == IR_LOOP_BEG && b->op == IR_ADD && (b->arg == 1 || b->arg == -1) && c->op == IR_LOOP_END)
        {
            a->op = 0;
            b->op = c->op = IR_NOP;
        }
    }
    pass_remove_nop_(prog);
}

void    pass_contract(IRProg *prog)
{
    IRProg  out = {0};
    for (size_t i = 0; i < prog->count; )
    {
        IRInstr cur = prog->instrs[i];

        if (cur.op == IR_ADD || cur.op == IR_MOVE)
        {
            int sum = cur.arg;
            size_t  j = i + 1;
            while (j < prog->count && prog->instrs[j].op == cur.op)
            {
                sum += prog->instrs[j].arg;
                ++j;
            }
            if (sum != 0)
                emit(&out, (IRInstr){cur.op, sum, 0});
            i = j;
        }
        else
        {
            emit(&out, cur);
            ++i;
        }
    }
    pass_clean_loop_(prog);
    free(prog->instrs);
    *prog = out;
}

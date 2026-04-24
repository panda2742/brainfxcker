#include "ir.h"
#include "parser.h"
#include <stddef.h>
#include <stdlib.h>

static void emit_(IRProg *prog, IRInstr instr)
{
    if (prog->count >= prog->capacity)
    {
        prog->capacity = prog->capacity ? prog->capacity * 2 : 64;
        prog->instrs = realloc(prog->instrs, prog->capacity * sizeof(IRInstr));
    }
    prog->instrs[prog->count++] = instr;
}

void gen_ir_nodes(IRProg *prog, Node *nodes, size_t n)
{
    for (size_t i = 0; i < n; ++i)
    {
        switch (nodes[i].kind)
        {
            case NODE_INC_PTR: emit_(prog, (IRInstr){IR_MOVE, +1, 0}); break;
            case NODE_DEC_PTR: emit_(prog, (IRInstr){IR_MOVE, -1, 0}); break;
            case NODE_INC_VAL: emit_(prog, (IRInstr){IR_ADD,  +1, 0}); break;
            case NODE_DEC_VAL: emit_(prog, (IRInstr){IR_ADD,  -1, 0}); break;
            case NODE_OUTPUT:  emit_(prog, (IRInstr){IR_OUTPUT, 0, 0});  break;
            case NODE_INPUT:   emit_(prog, (IRInstr){IR_INPUT, 0, 0});   break;
            case NODE_LOOP:
            {
                size_t  open_idx = prog->count;
                emit_(prog, (IRInstr){IR_LOOP_BEG, 0, 0});
                gen_ir_nodes(prog, nodes[i].children, nodes[i].child_count);
                size_t  close_idx = prog->count;
                emit_(prog, (IRInstr){IR_LOOP_END, (int)open_idx, 0});
                prog->instrs[open_idx].arg = (int)close_idx;
                break;
            }
        }
    }
}

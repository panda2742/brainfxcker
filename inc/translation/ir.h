#pragma once

#include "init/parser.h"
#include <stddef.h>

typedef enum {
	IR_ADD,		 // *ptr += arg	 (peut être négatif)
	IR_MOVE,	 // ptr += arg	 (peut être négatif)
	IR_SET,		 // *ptr = arg	 (généré après optimisation)
	IR_OUTPUT,	 // write(1, ptr, 1)
	IR_INPUT,	 // read(0, ptr, 1)
	IR_LOOP_BEG, // if (*ptr == 0) goto LOOP_END[target]
	IR_LOOP_END, // if (*ptr != 0) goto LOOP_BEG[target]
	IR_MUL_COPY, // ptr[offset] += *ptr * factor  (optimisation)
	IR_NOP,		 // (optimisation uniquement)
}	IROp;

typedef struct {
	IROp	op;
	int		arg;	 // valeur / delta / target selon l'opcode
	int		offset;	 // pour IR_MUL_COPY
}	IRInstr;

void gen_ir_nodes(PagedVector *prog, Node *nodes, size_t n);

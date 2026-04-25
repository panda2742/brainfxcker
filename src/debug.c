#include "debug.h"
#include "init/parser.h"
#include <stdio.h>
#include <stdlib.h>

#define COLOR_RESET	  "\033[0m"
#define COLOR_GRAY	  "\033[90m"
#define COLOR_BLUE	  "\033[94m"
#define COLOR_GREEN	  "\033[92m"
#define COLOR_YELLOW  "\033[93m"
#define COLOR_MAGENTA "\033[95m"
#define COLOR_RED	  "\033[91m"
#define COLOR_CYAN	  "\033[96m"

static const char* token_kind_to_str(TokenKind kind) {
	switch(kind) {
		case TOK_INC_PTR:	return ">";
		case TOK_DEC_PTR:	return "<";
		case TOK_INC_VAL:	return "+";
		case TOK_DEC_VAL:	return "-";
		case TOK_OUTPUT:	return ".";
		case TOK_INPUT:		return ",";
		case TOK_LOOP_OPEN: return "[";
		case TOK_LOOP_CLOSE:return "]";
		default:			return "?";
	}
}

static const char* node_kind_to_str(NodeKind kind) {
	switch(kind) {
		case NODE_INC_PTR:	 return "INC_PTR";
		case NODE_DEC_PTR:	 return "DEC_PTR";
		case NODE_INC_VAL:	 return "INC_VAL";
		case NODE_DEC_VAL:	 return "DEC_VAL";
		case NODE_OUTPUT:	 return "OUTPUT";
		case NODE_INPUT:	 return "INPUT";
		case NODE_LOOP:		 return "LOOP";
		default:			 return "?";
	}
}

void debug_lexer(TokenList *tokens) {
	printf("\n%s━━━ LEXER OUTPUT ━━━%s\n", COLOR_GRAY, COLOR_RESET);
	printf("%s%4s │ %-10s │ %s%s\n", COLOR_GRAY, "idx", "token", "loc", COLOR_RESET);
	printf("%s────┼────────────┼──────%s\n", COLOR_GRAY, COLOR_RESET);

	for (size_t i = 0; i < tokens->count; i++) {
		Token t = tokens->data[i];
		const char* symbol = token_kind_to_str(t.kind);

		const char* color;
		switch(t.kind) {
			case TOK_INC_PTR: case TOK_DEC_PTR: color = COLOR_BLUE; break;
			case TOK_INC_VAL: case TOK_DEC_VAL: color = COLOR_GREEN; break;
			case TOK_OUTPUT:  case TOK_INPUT:	color = COLOR_YELLOW; break;
			case TOK_LOOP_OPEN: case TOK_LOOP_CLOSE: color = COLOR_MAGENTA; break;
			default: color = COLOR_GRAY; break;
		}

		printf("  %s%3zu%s │   %s%s%s	│ %zu:%zu\n",
			   COLOR_GRAY, i, COLOR_RESET,
			   color, symbol, COLOR_RESET,
			   t.line, t.col);
	}

	printf("%s━━━ total: %zu tokens ━━━%s\n\n", COLOR_GRAY, tokens->count, COLOR_RESET);
}

static void debug_node(const Node* node, int depth) {
	const char* indent = "	";
	for (int i = 0; i < depth; i++) printf("%s", indent);

	const char* color;
	switch(node->kind) {
		case NODE_INC_PTR: case NODE_DEC_PTR: color = COLOR_BLUE; break;
		case NODE_INC_VAL: case NODE_DEC_VAL: color = COLOR_GREEN; break;
		case NODE_OUTPUT:  case NODE_INPUT:	  color = COLOR_YELLOW; break;
		case NODE_LOOP:						   color = COLOR_MAGENTA; break;
		default: color = COLOR_GRAY; break;
	}

	printf("%s%s%s", color, node_kind_to_str(node->kind), COLOR_RESET);

	if (node->kind == NODE_LOOP) {
		printf(" %s[%zu child%s]%s",
			   COLOR_GRAY, node->child_count, node->child_count == 1 ? "" : "ren", COLOR_RESET);
	}

	printf(" %s(%zu:%zu)%s\n", COLOR_GRAY, node->line, node->col, COLOR_RESET);

	if (node->kind == NODE_LOOP) {
		for (size_t i = 0; i < node->child_count; i++) {
			debug_node(&node->children[i], depth + 1);
		}
	}
}

void debug_parser(AST *ast) {
	if (!ast || ast->count == 0) {
		printf("\n%s━━━ PARSER OUTPUT ━━━%s\n", COLOR_GRAY, COLOR_RESET);
		printf("%s	(empty program)%s\n\n", COLOR_YELLOW, COLOR_RESET);
		return;
	}

	printf("\n%s━━━ PARSER OUTPUT ━━━%s\n", COLOR_GRAY, COLOR_RESET);

	for (size_t i = 0; i < ast->count; i++) {
		debug_node(&ast->nodes[i], 0);
	}

	printf("%s━━━ total: %zu nodes ━━━%s\n\n", COLOR_GRAY, ast->count, COLOR_RESET);
}

static const char* ir_op_to_str(IROp op) {
	switch(op) {
		case IR_ADD:	   return "ADD";
		case IR_MOVE:	   return "MOVE";
		case IR_SET:	   return "SET";
		case IR_OUTPUT:	   return "OUTPUT";
		case IR_INPUT:	   return "INPUT";
		case IR_LOOP_BEG:  return "LOOP_BEG";
		case IR_LOOP_END:  return "LOOP_END";
		case IR_MUL_COPY:  return "MUL_COPY";
		default:		   return "?";
	}
}

void debug_ir(IRProg *ir) {
	if (!ir || ir->count == 0) {
		printf("\n%s━━━ IR OUTPUT ━━━%s\n", COLOR_GRAY, COLOR_RESET);
		printf("%s	(empty IR)%s\n\n", COLOR_YELLOW, COLOR_RESET);
		return;
	}

	printf("\n%s━━━ IR OUTPUT ━━━%s\n", COLOR_GRAY, COLOR_RESET);
	printf("%s%-4s │ %-10s │ %-8s │ %s%s\n",
		   COLOR_GRAY, "idx", "op", "arg", "offset", COLOR_RESET);
	printf("%s────┼────────────┼──────────┼────────%s\n", COLOR_GRAY, COLOR_RESET);

	for (size_t i = 0; i < ir->count; i++) {
		IRInstr instr = ir->instrs[i];

		// Couleur selon le type d'opération
		const char* color;
		switch(instr.op) {
			case IR_ADD:  case IR_SET:	  color = COLOR_GREEN; break;
			case IR_MOVE:				 color = COLOR_BLUE;  break;
			case IR_OUTPUT: case IR_INPUT: color = COLOR_YELLOW; break;
			case IR_LOOP_BEG: case IR_LOOP_END: color = COLOR_MAGENTA; break;
			case IR_MUL_COPY:			 color = COLOR_CYAN;  break;
			default: color = COLOR_GRAY; break;
		}

		// Affichage de l'argument avec signe pour ADD et MOVE
		if (instr.op == IR_ADD || instr.op == IR_MOVE) {
			char sign = instr.arg >= 0 ? '+' : '-';
			printf("  %s%3zu%s │ %s%-10s%s │   %s%c%d%s	   │ %s%8d%s\n",
				   COLOR_GRAY, i, COLOR_RESET,
				   color, ir_op_to_str(instr.op), COLOR_RESET,
				   COLOR_GREEN, sign, abs(instr.arg), COLOR_RESET,
				   COLOR_GRAY, instr.offset, COLOR_RESET);
		}
		// Pour LOOP_BEG/LOOP_END, afficher la cible
		else if (instr.op == IR_LOOP_BEG || instr.op == IR_LOOP_END) {
			printf("  %s%3zu%s │ %s%-10s%s │ → %s%-6d%s │ %s%8d%s\n",
				   COLOR_GRAY, i, COLOR_RESET,
				   color, ir_op_to_str(instr.op), COLOR_RESET,
				   COLOR_MAGENTA, instr.arg, COLOR_RESET,
				   COLOR_GRAY, instr.offset, COLOR_RESET);
		}
		// Pour les autres
		else {
			if (instr.arg != 0 && instr.op != IR_SET) {
				printf("  %s%3zu%s │ %s%-10s%s │   %s%-6d%s │ %s%8d%s\n",
					   COLOR_GRAY, i, COLOR_RESET,
					   color, ir_op_to_str(instr.op), COLOR_RESET,
					   COLOR_CYAN, instr.arg, COLOR_RESET,
					   COLOR_GRAY, instr.offset, COLOR_RESET);
			} else if (instr.op == IR_SET) {
				printf("  %s%3zu%s │ %s%-10s%s │   %s%-6d%s │ %s%8d%s\n",
					   COLOR_GRAY, i, COLOR_RESET,
					   color, ir_op_to_str(instr.op), COLOR_RESET,
					   COLOR_GREEN, instr.arg, COLOR_RESET,
					   COLOR_GRAY, instr.offset, COLOR_RESET);
			} else {
				printf("  %s%3zu%s │ %s%-10s%s │   %-8s │ %s%8d%s\n",
					   COLOR_GRAY, i, COLOR_RESET,
					   color, ir_op_to_str(instr.op), COLOR_RESET,
					   "—",
					   COLOR_GRAY, instr.offset, COLOR_RESET);
			}
		}
	}

	printf("%s━━━ total: %zu instructions ━━━%s\n\n", COLOR_GRAY, ir->count, COLOR_RESET);
}

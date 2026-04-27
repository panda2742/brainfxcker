#pragma once

#include "utils/paged_vector.h"
#include <stddef.h>

typedef enum {
	NODE_INC_PTR,
	NODE_DEC_PTR,
	NODE_INC_VAL,
	NODE_DEC_VAL,
	NODE_OUTPUT,
	NODE_INPUT,
	NODE_LOOP
} NodeKind;

typedef struct Node {
	NodeKind	 kind;
	struct Node *children;
	size_t		 child_count;
	size_t		 line, col;
} Node;

typedef struct {
	Node	*nodes;
	size_t	count;
} AST;

void	free_nodes(Node *nodes, size_t count);
size_t	 parse_block(PagedVector *tokens, size_t start, size_t len, Node **out, size_t *out_count);

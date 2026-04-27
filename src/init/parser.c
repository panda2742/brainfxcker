#include "init/parser.h"
#include "init/lexer.h"
#include "utils/paged_vector.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void	free_nodes(Node *nodes, size_t count)
{
	for (size_t i = 0; i < count; ++i)
	{
		if (nodes[i].kind == NODE_LOOP)
			free_nodes(nodes[i].children, nodes[i].child_count);
	}
	free(nodes);
}

size_t	parse_block(PagedVector *tokens, size_t start, size_t len, Node **out, size_t *out_count)
{
	Node   *buf = NULL;
	size_t  buf_cap = 0, buf_len = 0;
	size_t  i = 0;

	while (i < len)
	{
		Token *tok = pv_get(tokens, start + i);

		if (tok->kind == TOK_LOOP_CLOSE) break;
		if (buf_len >= buf_cap)
		{
			buf_cap = buf_cap ? buf_cap * 2 : 32;
			buf = realloc(buf, buf_cap * sizeof(Node));
		}

		Node node = {.kind = (NodeKind)tok->kind, .line = tok->line, .col = tok->col};

		if (tok->kind == TOK_LOOP_OPEN)
		{
			size_t	consumed = parse_block(tokens, start + i + 1, len - i - 1, &node.children, &node.child_count);
			Token	*close = pv_get(tokens, start + i + 1 + consumed);
			if (close->kind != TOK_LOOP_CLOSE)
			{
				fprintf(stderr, "error: unmatched '[' at %zu:%zu\n", tok->line, tok->col);
				exit(1);
			}
			i += consumed + 2;
		}
		else ++i;
		buf[buf_len++] = node;
	}

	*out = buf;
	*out_count = buf_len;
	return i;
}

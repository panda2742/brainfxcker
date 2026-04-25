#include "init/parser.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

size_t	 parse_block(Token *tokens, size_t len, Node **out, size_t *out_count)
{
	Node	*buf = NULL;
	size_t	buf_cap = 0, buf_len = 0;
	size_t	i = 0;

	while (i < len)
	{
		Token	tok = tokens[i];

		if (tok.kind == TOK_LOOP_CLOSE) break;
		if (buf_len >= buf_cap)
		{
			buf_cap = buf_cap ? buf_cap * 2 : 32;
			buf = realloc(buf, buf_cap * sizeof(Node));
		}

		Node	node = {.kind = (NodeKind)tok.kind, .line = tok.line, .col = tok.col};

		if (tok.kind == TOK_LOOP_OPEN)
		{
			size_t	consumed = parse_block(tokens + i + 1, len - i - 1, &node.children, &node.child_count);
			if (tokens[i + 1 + consumed].kind != TOK_LOOP_CLOSE)
			{
				fprintf(stderr, "error: unmatched '[' at %zu:%zu\n", tok.line, tok.col);
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

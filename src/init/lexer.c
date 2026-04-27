#include "init/lexer.h"
#include "utils/paged_vector.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

PagedVector	*lex_file(const char *path)
{
	FILE	*f = fopen(path, "r");
	if (!f)
	{
		perror(path);
		exit(EXIT_FAILURE);
	}

	PagedVector	*tl = pv_create(sizeof(Token));
	size_t		line = 1, col = 0;
	int			c;

	while ((c = fgetc(f)) != EOF)
	{
		++col;
		if (c == '\n')
		{
			++line;
			col = 0;
			continue;
		}

		TokenKind	kind;
		switch (c)
		{
			case '>': kind = TOK_INC_PTR;	break;
			case '<': kind = TOK_DEC_PTR;	break;
			case '+': kind = TOK_INC_VAL;	break;
			case '-': kind = TOK_DEC_VAL;	break;
			case '.': kind = TOK_OUTPUT;	break;
			case ',': kind = TOK_INPUT;		break;
			case '[': kind = TOK_LOOP_OPEN; break;
			case ']': kind = TOK_LOOP_CLOSE;break;
			default: continue;
		}
		pv_push(tl, &(Token){kind, line, col});
	}
	fclose(f);
	return tl;
}

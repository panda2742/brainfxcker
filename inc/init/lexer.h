#pragma once

#include <stddef.h>

typedef enum {
	TOK_INC_PTR,   // >
	TOK_DEC_PTR,   // <
	TOK_INC_VAL,   // +
	TOK_DEC_VAL,   // -
	TOK_OUTPUT,	   // .
	TOK_INPUT,	   // ,
	TOK_LOOP_OPEN, // [
	TOK_LOOP_CLOSE // ]
} TokenKind;

typedef struct {
	TokenKind	kind;
	size_t		line;
	size_t		col;
}	Token;

typedef struct {
	Token	*data;
	size_t	count;
	size_t	capacity;
}	TokenList;

TokenList	lex_file(const char *path);
void		free_token_list(TokenList *tl);

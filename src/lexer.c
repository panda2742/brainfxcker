#include "lexer.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static void tl_push_(TokenList *tl, Token t)
{
    if (tl->count >= tl->capacity)
    {
        tl->capacity = tl->capacity ? tl->capacity * 2 : 256;
        tl->data = realloc(tl->data, tl->capacity * sizeof(Token));
    }
    tl->data[tl->count++] = t;
}

TokenList   lex_file(const char *path)
{
    FILE    *f = fopen(path, "r");
    if (!f)
    {
        perror(path);
        exit(EXIT_FAILURE);
    }

    TokenList   tl = {0};
    size_t      line = 1, col = 0;
    int         c;

    while ((c = fgetc(f)) != EOF)
    {
        ++col;
        if (c == '\n')
        {
            ++line;
            col = 0;
            continue;
        }

        TokenKind   kind;
        switch (c)
        {
            case '>': kind = TOK_INC_PTR;   break;
            case '<': kind = TOK_DEC_PTR;   break;
            case '+': kind = TOK_INC_VAL;   break;
            case '-': kind = TOK_DEC_VAL;   break;
            case '.': kind = TOK_OUTPUT;    break;
            case ',': kind = TOK_INPUT;     break;
            case '[': kind = TOK_LOOP_OPEN; break;
            case ']': kind = TOK_LOOP_CLOSE;break;
            default: continue;
        }
        tl_push_(&tl, (Token){kind, line, col});
    }
    fclose(f);
    return tl;
}

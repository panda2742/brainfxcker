#include "output.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Output  get_output(const char *path)
{
    char *dot = strrchr(path, '.');
    if (dot && strcmp(dot, ".bf") == 0)
    {
        dot[1] = 's';
        dot[2] = '\0';
    }
    size_t  base_len = dot - path;
    Output  res = {.len=base_len,.asm_out_path=NULL,.base=NULL,.asm_out=NULL};
    res.base = malloc(base_len + 1);
    if (!res.base) return res;

    res.base[base_len] = 0;
    memcpy(res.base, path, base_len - 1);

    res.asm_out_path = malloc(base_len + 3);
    sprintf(res.asm_out_path, "%s.s", res.base);
    res.asm_out = fopen(res.asm_out_path, "w");

    return res;
}

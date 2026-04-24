#pragma once

#include <stdio.h>
#include <stddef.h>

typedef struct {
    char    *base;
    size_t  len;
    char    *asm_out_path;
    FILE    *asm_out;
}   Output;

Output  get_output(const char *path);

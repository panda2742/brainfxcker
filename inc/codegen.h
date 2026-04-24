#pragma once

#include "ir.h"
#include <stddef.h>
#include <stdio.h>

extern size_t   loop_id;

FILE    *create_output(const char *path, char **output_path);
int     write_instructions(FILE *out, IRProg *prog);

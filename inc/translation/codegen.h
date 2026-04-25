#pragma once

#include "ir.h"
#include <stddef.h>
#include <stdio.h>

extern size_t	loop_id;

int		write_instructions(FILE *out, IRProg *prog);

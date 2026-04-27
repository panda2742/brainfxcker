#pragma once

#include "utils/paged_vector.h"
#include <stddef.h>
#include <stdio.h>

extern size_t	loop_id;

int		write_instructions(FILE *out, PagedVector *prog);

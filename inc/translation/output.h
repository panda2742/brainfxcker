#pragma once

#include <stdio.h>
#include <stddef.h>

typedef enum {
	OUTPUT_ASM,
	OUTPUT_OBJECT,
}	OutputDataType;

typedef struct {
	OutputDataType	type;
	char			*path;
	size_t			path_len;
	char			*cmd;
	size_t			cmd_len;
	FILE			*file;
}	OutputData;

typedef struct {
	OutputData	asm;
	OutputData	object;
}	Output;

Output	create_output(const char *path);
void	free_output(Output *out);
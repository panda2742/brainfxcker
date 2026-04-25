#define _POSIX_C_SOURCE 200809L
#include "translation/output.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OUTPUT_ASM_CMD "nasm -f elf64 "
#define OUTPUT_ASM_CMD_LEN 14
#define OUTPUT_ASM_EXT "s"
#define OUTPUT_ASM_EXT_LEN 1

#define OUTPUT_OBJECT_CMD "ld "
#define OUTPUT_OBJECT_CMD_LEN 3
#define OUTPUT_OBJECT_EXT "o"
#define OUTPUT_OBJECT_EXT_LEN 1

static const char	*prepare_base_path_(const char *path)
{
	char	resolved[4096];
	if (!realpath(path, resolved)) return NULL;

	char *dot = strrchr(resolved, '.');
	if (!dot) return NULL;
	dot[1] = 0;
	const char	*base_path = strdup(resolved);
	return base_path;
}

static OutputData	generate_out_data_(OutputDataType type, const char *base_path)
{
	const size_t	base_len = strlen(base_path);
	OutputData		data = {.type = type};
	char			*cmd = NULL, *ext = NULL;
	size_t			cmd_len = 0, ext_len = 0;

	switch (type)
	{
		case OUTPUT_ASM:
		cmd = OUTPUT_ASM_CMD;
		cmd_len = OUTPUT_ASM_CMD_LEN;
		ext = OUTPUT_ASM_EXT;
		ext_len = OUTPUT_ASM_EXT_LEN;
		break;
		case OUTPUT_OBJECT:
		cmd = OUTPUT_OBJECT_CMD;
		cmd_len = OUTPUT_OBJECT_CMD_LEN;
		ext = OUTPUT_OBJECT_EXT;
		ext_len = OUTPUT_OBJECT_EXT_LEN;
		break;
	}
	data.path_len = base_len + ext_len;
	data.path = malloc(base_len + 1);
	memcpy(data.path, base_path, base_len);
	memcpy(&data.path[base_len], ext, ext_len);
	data.path[data.path_len] = 0;

	data.cmd_len = cmd_len + data.path_len;
	data.cmd = malloc(data.cmd_len + 1);
	memcpy(data.cmd, cmd, cmd_len);
	memcpy(&data.cmd[cmd_len], data.path, data.path_len);
	data.cmd[data.cmd_len] = 0;
	if (data.type == OUTPUT_ASM)
		data.file = fopen(data.path, "w");
	return data;
}

Output	create_output(const char *path)
{
	const char	*base_path = prepare_base_path_(path);
	if (!base_path) { /* to do */ }

	Output	res;
	res.asm = generate_out_data_(OUTPUT_ASM, base_path);
	res.object = generate_out_data_(OUTPUT_OBJECT, base_path);
	return res;
}

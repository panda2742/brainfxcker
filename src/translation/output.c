#include "translation/output.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

#define OUTPUT_ASM_CMD "nasm -f elf64 "
#define OUTPUT_ASM_CMD_LEN 14
#define OUTPUT_ASM_EXT "s"
#define OUTPUT_ASM_EXT_LEN 1

#define OUTPUT_OBJECT_CMD "ld "
#define OUTPUT_OBJECT_CMD_LEN 3
#define OUTPUT_OBJECT_EXT "o"
#define OUTPUT_OBJECT_EXT_LEN 1

static char	*prepare_base_path_(const char *path)
{
	char	resolved[4096];
	if (!realpath(path, resolved)) return NULL;

	char *dot = strrchr(resolved, '.');
	if (!dot) return NULL;
	dot[1] = 0;
	char	*base_path = strdup(resolved);
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
	data.path = malloc(data.path_len + 1);
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
	char	*base_path = prepare_base_path_(path);
	if (!base_path) { /* to do */ }

	Output	res;
	res.asm = generate_out_data_(OUTPUT_ASM, base_path);
	res.object = generate_out_data_(OUTPUT_OBJECT, base_path);
	free(base_path);
	return res;
}

void	free_output(Output *out)
{
	if (out->asm.path)
		free(out->asm.path);
	if (out->asm.cmd)
		free(out->asm.cmd);
	if (out->object.path)
		free(out->object.path);
	if (out->object.cmd)
		free(out->object.cmd);
}

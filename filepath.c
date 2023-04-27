#include "shell.h"

int cant_open(char *file_path);
int proc_file_commands(char *file_path, int *exe_ret);

/**
 * cant_open - If file dont exist,it cant open.
 * @file_path: Path to file.
 * Return: 127.
 */

int cant_open(char *file_path)
{
	char *err, *h;
	int length;

	h = _itoa(hist);
	if (!h)
		return (127);
	length = _strlen(name) + _strlen(h) + _strlen(file_path) + 16;
	err = malloc(sizeof(char) * (length + 1));
	if (!err)
	{
		free(h);
		return (127);
	}

	_strcpy(err, name);
	_strcat(err, ": ");
	_strcat(err, h);
	_strcat(err, ": Can't open ");
	_strcat(err, file_path);
	_strcat(err, "\n");

	free(h);
	write(STDERR_FILENO, err, length);
	free(err);
	return (127);
}

/**
 * proc_file_commands - Attempts to run the commands that is stored
 * @file_path: Path to the file.
 * @exe_ret: Return value of the last executed command.
 *
 * Return: If file can't open.
 */
int proc_file_commands(char *file_path, int *exe_ret)
{
	ssize_t i, File, b;
	unsigned int Line = 0;
	unsigned int size = 120;
	char *l, **arg, **f;
	char buffer[120];
	int retn;

	hist = 0;
	File = open(file_path, O_RDONLY);
	if (File == -1)
	{
		*exe_ret = cant_open(file_path);
		return (*exe_ret);
	}
	l = malloc(sizeof(char) * size);
	if (!l)
		return (-1);
	do {
	b = read(File, buffer, 119);
		if (b == 0 && Line == 0)
			return (*exe_ret);
		buffer[b] = '\0';
		Line += b;
		l = _realloc(l, size, Line);
		_strcat(l, buffer);
		size = Line;
	} while (b);
	for (i = 0; l[i] == '\n'; i++)
		l[i] = ' ';
	for (; i < Line; i++)
	{
		if (l[i] == '\n')
		{
			l[i] = ';';
			for (i += 1; i < Line && l[i] == '\n'; i++)
				l[i] = ' ';
		}
	}
	variable_replacement(&l, exe_ret);
	handle_line(&l, Line);
	arg = _strtok(l, " ");
	free(l);
	if (!arg)
		return (0);
	if (check_args(arg) != 0)
	{
		*exe_ret = 2;
		free_args(arg, arg);
		return (*exe_ret);
	}
	f = arg;

	for (i = 0; arg[i]; i++)
	{
		if (_strncmp(arg[i], ";", 1) == 0)
		{
			free(arg[i]);
			arg[i] = NULL;
			retn = call_args(arg, f, exe_ret);
			arg = &arg[++i];
			i = 0;
		}
	}

	retn = call_args(arg, f, exe_ret);

	free(f);
	return (retn);
}

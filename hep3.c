#include "shell.h"

void free_args(char **args, char **front);
char *get_env_value(char *beginning, int len);
char *get_pid(void);
void variable_replacement(char **args, int *exe_ret);

/**
 * get_env_value - Recieves the value correspondence to an envar.
 * @beginning: The envar to be search for.
 * @len: The length of the environmental variable to search for.
 *
 * Return: If the variable is not found - an empty the value of the envar.
 *
 */
char *get_env_value(char *beginning, int len)
{
	char **addr;
	char *replace = NULL, *i, *variable;

	variable = malloc(len + 1);
	if (!variable)
		return (NULL);
	variable[0] = '\0';
	_strncat(variable, beginning, len);

	addr = _getenv(variable);
	free(variable);
	if (addr)
	{
		i = *addr;
		while (*i != '=')
			i++;
		i++;
		replace = malloc(_strlen(i) + 1);
		if (replace)
			_strcpy(replace, i);
	}

	return (replace);
}

/**
 * get_pid - Receive the current process Identification..
 *
 * Return: The current process ID or NULL on failure.
 */
char *get_pid(void)
{
	size_t temp = 0;
	char *buffer;
	ssize_t f;

	f = open("/proc/self/stat", O_RDONLY);
	if (f == -1)
	{
		perror("Cant read file");
		return (NULL);
	}
	buffer = malloc(120);
	if (!buffer)
	{
		close(f);
		return (NULL);
	}
	read(f, buffer, 120);
	while (buffer[temp] != ' ')
		temp++;
	buffer[temp] = '\0';

	close(f);
	return (buffer);
}

/**
 * variable_replacement - Handle variable to replace.
 * @line: A pointer that contains command.
 * @exe_ret: A return value of the last executed command.
 *
 * Description: To replace $$ with the current PID, $ return value
 *
 */
void variable_replacement(char **line, int *exe_ret)
{
	int j, k = 0, length;
	char *replace = NULL, *oline = NULL, *nline;

	oline = *line;
	for (j = 0; oline[j]; j++)
	{
		if (oline[j] == '$' && oline[j + 1] &&
		    oline[j + 1] != ' ')
		{
			if (oline[j + 1] == '$')
			{
				replace = get_pid();
				k = j + 2;
			}
			else if (oline[j + 1] == '?')
			{
				replace = _itoa(*exe_ret);
				k = j + 2;
			}
			else if (oline[j + 1])
			{
				for (k = j + 1; oline[k] &&
					     oline[k] != '$' &&
					     oline[k] != ' '; k++)
					;
				length = k - (j + 1);
				replace = get_env_value(&oline[j + 1], length);
			}
			nline = malloc(j + _strlen(replace)
					  + _strlen(&oline[k]) + 1);
			if (!line)
				return;
			nline[0] = '\0';
			_strncat(nline, oline, j);
			if (replace)
			{
				_strcat(nline, replace);
				free(replace);
				replace = NULL;
			}
			_strcat(nline, &oline[k]);
			free(oline);
			*line = nline;
			oline = nline;
			j = -1;
		}
	}
}
/**
 * free_args - Function releases the memory space.
 * @args: Null-terminated pointer containing commands&arguments.
 * @front: Apointer to the beginning of arguments.
 */
void free_args(char **args, char **front)
{
	size_t k;

	for (k = 0; args[k] || args[k + 1]; k++)
		free(args[k]);

	free(front);
}


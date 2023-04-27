#include "shell.h"

char *get_args(char *line, int *exe_ret);
int run_args(char **args, char **front, int *exe_ret);
int handle_args(int *exe_ret);
int check_args(char **args);
int call_args(char **args, char **front, int *exe_ret);

/**
 * get_args - Recieve a command from standard input.
 * @line: A buffeliner to store the command.
 *
 * @exe_ret: The value returned.
 *
 * Return: NULL if an error occurs
 */
char *get_args(char *line, int *exe_ret)
{
	size_t size = 0;
	char *prompt = "$ ";
	ssize_t Read;

	if (line)
		free(line);
	Read = _getline(&line, &size, STDIN_FILENO);
	if (Read == -1)
		return (NULL);
	if (Read == 1)
	{
		hist++;
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, prompt, 2);
		return (get_args(line, exe_ret));
	}

	line[Read - 1] = '\0';
	variable_replacement(&line, exe_ret);
	handle_line(&line, Read);

	return (line);
}
/**
 * check_args - Checks if there are  arguments ','; ';;', '&&', or '||'.
 * @args: A  pointer tokenized commands and arguments.
 *
 * Return: return -2 if it takes an argument else 0.
 */
int check_args(char **args)
{
	size_t index;
	char *vik, *lif;

	for (index = 0; args[index]; index++)
	{
		vik = args[index];
		if (vik[0] == ';' || vik[0] == '&' || vik[0] == '|')
		{
			if (index == 0 || vik[1] == ';')
				return (create_error(&args[index], 2));
			lif = args[index + 1];
			if (lif && (lif[0] == ';' || lif[0] == '&' || lif[0] == '|'))
				return (create_error(&args[index + 1], 2));
		}
	}
	return (0);
}

/**
 * call_args - Spliting operators.
 * @args: An array of arguments.
 * @front: A pointer to args.
 * @exe_ret: The return value of the parent process' last executed command.
 *
 *
 * Return: The value of the last executed command.
 */
int call_args(char **args, char **front, int *exe_ret)
{
	int i, Ret;

	if (!args[0])
		return (*exe_ret);
	for (i = 0; args[i]; i++)
	{
		if (_strncmp(args[i], "||", 2) == 0)
		{
			free(args[i]);
			args[i] = NULL;
			args = replace_aliases(args);
			Ret = run_args(args, front, exe_ret);
			if (*exe_ret != 0)
			{
				args = &args[++i];
				i = 0;
			}
			else
			{
				for (i++; args[i]; i++)
					free(args[i]);
				return (Ret);
			}
		}
		else if (_strncmp(args[i], "&&", 2) == 0)
		{
			free(args[i]);
			args[i] = NULL;
			args = replace_aliases(args);
			Ret = run_args(args, front, exe_ret);
			if (*exe_ret == 0)
			{
				args = &args[++i];
				i = 0;
			}
			else
			{
				for (i++; args[i]; i++)
					free(args[i]);
				return (Ret);
			}
		}
	}
	args = replace_aliases(args);
	Ret = run_args(args, front, exe_ret);
	return (Ret);
}

/**
 * run_args - Callings to execute a command.
 * @args: An array of arguments.
 * @front: A pointerto the args.
 * @exe_ret: It returns the value parent process'.
 *
 *
 * Return: The return value of the l. exec command.
 */
int run_args(char **args, char **front, int *exe_ret)
{
	int Ret, index;
	int (*builtin)(char **args, char **front);

	builtin = get_builtin(args[0]);

	if (builtin)
	{
		Ret = builtin(args + 1, front);
		if (Ret != EXIT)
			*exe_ret = Ret;
	}
	else
	{
		*exe_ret = execute(args, front);
		Ret = *exe_ret;
	}
	hist++;
	for (index = 0; args[index]; index++)
		free(args[index]);
	return (Ret);
}

/**
 * handle_args - Call,get and runs the execution of a command.
 * @exe_ret: value of the parent process' last executed command.
 *
 * Return: execur return
 */
int handle_args(int *exe_ret)
{
	int j, ret = 0;
	char **arg, *line = NULL, **f;

	line = get_args(line, exe_ret);
	if (!line)
		return (END_OF_FILE);
	arg = _strtok(line, " ");
	free(line);
	if (!arg)
		return (ret);
	if (check_args(arg) != 0)
	{
		*exe_ret = 2;
		free_args(arg, arg);
		return (*exe_ret);
	}
	f = arg;

	for (j = 0; arg[j]; j++)
	{
		if (_strncmp(arg[j], ";", 1) == 0)
		{
			free(arg[j]);
			arg[j] = NULL;
			ret = call_args(arg, f, exe_ret);
			arg = &arg[++j];
			j = 0;
		}
	}
	if (arg)
		ret = call_args(arg, f, exe_ret);

	free(f);
	return (ret);
}

#include "shell.h"

int shellby_exit(char **args, char **front);
int shellby_help(char **args, char __attribute__((__unused__)) **front);
int shellby_cd(char **args, char __attribute__((__unused__)) **front);
int (*get_builtin(char *command))(char **args, char **front);
/**
 * get_builtin - Command for shell builtins.
 * @command: Command to makematch.
 * Return: a pointer of functions to corresponding builtin.
 */
int (*get_builtin(char *command))(char **args, char **front)
{
	builtin_t functions[] = {
		{ "exit", shellby_exit },
		{ "cd", shellby_cd },
		{ "unsetenv", shellby_unsetenv },
		{ "env", shellby_env },
		{ "setenv", shellby_setenv },
		{ "alias", shellby_alias },
		{ "help", shellby_help },
		{ NULL, NULL }
	};
	int k;

	for (k = 0; functions[k].name; k++)
	{
		if (_strcmp(functions[k].name, command) == 0)
			break;
	}
	return (functions[k].f);
}

/**
 * shellby_exit - This auses normal process to terminate
 * @args: An array of arguments.
 * @front: A pointer to the beginning of args.
 *
 * Return: If there are no arguments
 */
int shellby_exit(char **args, char **front)
{
	int w = 0, length_of_integer = 10;
	unsigned int max = 1 << (sizeof(int) * 8 - 1), val = 0;

	if (args[0])
	{
		if (args[0][0] == '+')
		{
			w = 1;
			length_of_integer++;
		}
		for (; args[0][w]; w++)
		{
			if (w <= length_of_integer && args[0][w] >= '0' && args[0][w] <= '9')
				val = (val * 10) + (args[0][w] - '0');
			else
				return (create_error(--args, 2));
		}
	}
	else
	{
		return (-3);
	}
	if (val > max - 1)
		return (create_error(--args, 2));
	args -= 1;
	free_args(args, front);
	free_env();
	free_alias_list(aliases);
	exit(val);
}

/**
 * shellby_help - Displays information about shellby builtin commands.
 * @args: An array of arguments.
 * @front: A pointer to the beginning of args.
 *
 * Return: If an error occurs -1 else 0.
 */
int shellby_help(char **args, char __attribute__((__unused__)) **front)
{
	if (!args[0])
		help_all();
	else if (_strcmp(args[0], "alias") == 0)
		help_alias();
	else if (_strcmp(args[0], "exit") == 0)
		help_exit();
	else if (_strcmp(args[0], "unsetenv") == 0)
		help_unsetenv();
	else if (_strcmp(args[0], "env") == 0)
		help_env();
	else if (_strcmp(args[0], "setenv") == 0)
		help_setenv();
	else if (_strcmp(args[0], "cd") == 0)
		help_cd();
	else if (_strcmp(args[0], "help") == 0)
		help_help();
	else
		write(STDERR_FILENO, name, _strlen(name));
	return (0);
}
/**
 * shellby_cd - This changes the current directory of this function process.
 * @args: An array of arguments.
 * @front: A pointer to the beginning of arguments.
 *
 * Return: If the string is not a directory
 */
int shellby_cd(char **args, char __attribute__((__unused__)) **front)
{
	char *new_line = "\n", **info;
	char *pwd = NULL, *o_pwd = NULL;
	struct stat dir;

	o_pwd = getcwd(o_pwd, 0);
	if (!o_pwd)
		return (-1);
	if (args[0])
	{
		if (*(args[0]) == '-' || _strcmp(args[0], "--") == 0)
		{
			if ((args[0][1] == '-' && args[0][2] == '\0') ||
					args[0][1] == '\0')
			{
				if (_getenv("OLDPWD") != NULL)
					(chdir(*_getenv("OLDPWD") + 7));
			}
			else
			{
				free(o_pwd);
				return (create_error(args, 2));
			}
		}
		else
		{
			if (stat(args[0], &dir) == 0 && S_ISDIR(dir.st_mode)
					&& ((dir.st_mode & S_IXUSR) != 0))
				chdir(args[0]);
			else
			{
				free(o_pwd);
				return (create_error(args, 2));
			}
		}
	}
	else
	{
		if (_getenv("HOME") != NULL)
			chdir(*(_getenv("HOME")) + 5);
	}
	pwd = getcwd(pwd, 0);
	if (!pwd)
		return (-1);
	info = malloc(sizeof(char *) * 2);
	if (!info)
		return (-1);
	info[0] = "OLDPWD";
	info[1] = o_pwd;
	if (shellby_setenv(info, info) == -1)
		return (-1);
	info[0] = "PWD";
	info[1] = pwd;
	if (shellby_setenv(info, info) == -1)
		return (-1);
	if (args[0] && args[0][0] == '-' && args[0][1] != '-')
	{
		write(STDOUT_FILENO, pwd, _strlen(pwd));
		write(STDOUT_FILENO, new_line, 1);
	}
	free(o_pwd);
	free(pwd);
	free(info);
	return (0);
}


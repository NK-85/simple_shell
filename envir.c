#include "shell.h"

int shellby_env(char **args, char __attribute__((__unused__)) **front);
int shellby_setenv(char **args, char __attribute__((__unused__)) **front);
int shellby_unsetenv(char **args, char __attribute__((__unused__)) **front);

/**
 * shellby_env - To print the current working environment.
 * @args: An array of argument.
 * @front: A  pointer of args.
 *
 * Return: -1 if error occurs
 *
 */
int shellby_env(char **args, char __attribute__((__unused__)) **front)
{
	int i;
	char nwch = '\n';

	if (!environ)
		return (-1);

	for (i = 0; environ[i]; i++)
	{
		write(STDOUT_FILENO, environ[i], _strlen(environ[i]));
		write(STDOUT_FILENO, &nwch, 1);
	}

	(void)args;
	return (0);
}

/**
 * shellby_setenv - Add an environments variable to the PATH.
 * @args: An array of arguments passed to the shell.
 * @front: Pointer to args.
 *
 * Return: Error occurs return -1 else 0.
 */
int shellby_setenv(char **args, char __attribute__((__unused__)) **front)
{
	char **p = NULL, **pp, *v;
	size_t n;
	int i;

	if (!args[0] || !args[1])
		return (create_error(args, -1));
	v = malloc(_strlen(args[0]) + 1 + _strlen(args[1]) + 1);
	if (!v)
		return (create_error(args, -1));
	_strcpy(v, args[0]);
	_strcat(v, "=");
	_strcat(v, args[1]);
	p = _getenv(args[0]);
	if (p)
	{
		free(*p);
		*p = v;
		return (0);
	}
	for (n = 0; environ[n]; n++)
		;
	pp = malloc(sizeof(char *) * (n + 2));
	if (!pp)
	{
		free(v);
		return (create_error(args, -1));
	}
	for (i = 0; environ[i]; i++)
		pp[i] = environ[i];
	free(environ);
	environ = pp;
	environ[i] = v;
	environ[i + 1] = NULL;
	return (0);
}

/**
 * shellby_unsetenv - This deletes an envir variable from the PATH.
 * @args: An array of arguments.
 * @front: A pointer of args.
 *
 * Return: Error returns -1 else 0.
 */
int shellby_unsetenv(char **args, char __attribute__((__unused__)) **front)
{
	char **p, **pp;
	size_t n;
	int i, j;

	if (!args[0])
		return (create_error(args, -1));
	p = _getenv(args[0]);
	if (!p)
		return (0);

	for (n = 0; environ[n]; n++)
		;

	pp = malloc(sizeof(char *) * n);
	if (!pp)
		return (create_error(args, -1));

	for (i = 0, j = 0; environ[i]; i++)
	{
		if (*p == environ[i])
		{
			free(*p);
			continue;
		}
		pp[j] = environ[i];
		j++;
	}
	free(environ);
	environ = pp;
	environ[n - 1] = NULL;

	return (0);
}

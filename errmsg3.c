#include "shell.h"
char *error_2_cd(char **args);
char *error_2_syntax(char **args);
char *error_env(char **args);
char *error_1(char **args);
char *error_2_exit(char **args);

/**
 * error_2_syntax - Generate an error message.
 * @args: An array of argument.
 *
 * Return: Error message.
 */
char *error_2_syntax(char **args)
{
	char *err, *str;
	int length;

	str = _itoa(hist);
	if (!str)
		return (NULL);
	length = _strlen(name) + _strlen(str) + _strlen(args[0]) + 33;
	err = malloc(sizeof(char) * (length + 1));
	if (!err)
	{
		free(str);
		return (NULL);
	}

	_strcpy(err, name);
	_strcat(err, ": ");
	_strcat(err, str);
	_strcat(err, ": Syntax error: \"");
	_strcat(err, args[0]);
	_strcat(err, "\" unexpected\n");

	free(str);
	return (err);
}
/**
 * error_env - Generate an error message.
 * @args: An array of arguments.
 *
 * Return: Error message.
 */
char *error_env(char **args)
{
	char *err, *str;
	int length;

	str = _itoa(hist);
	if (!str)
		return (NULL);
	args--;
	length = _strlen(name) + _strlen(str) + _strlen(args[0]) + 45;
	err = malloc(sizeof(char) * (length + 1));
	if (!err)
	{
		free(str);
		return (NULL);
	}
	_strcpy(err, name);
	_strcat(err, ": ");
	_strcat(err, str);
	_strcat(err, ": ");
	_strcat(err, args[0]);
	_strcat(err, ": Unable to add/remove from environment\n");

	free(str);
	return (err);
}

/**
 * error_1 - Generates an error message.
 *  @args: An array of argument.
 *
 *
 * Return: Error message.
 */
char *error_1(char **args)
{
	char *err;
	int length;

	length = _strlen(name) + _strlen(args[0]) + 13;
	err = malloc(sizeof(char) * (length + 1));
	if (!err)
		return (NULL);
	_strcpy(err, "alias: ");
	_strcat(err, args[0]);
	_strcat(err, " not found\n");
	return (err);
}

/**
 * error_2_cd - Generates an error message.
 * @args: An array of argument.
 *
 * Return: Error message.
 */
char *error_2_cd(char **args)
{
	char *err, *str;
	int length;

	str = _itoa(hist);
	if (!str)
		return (NULL);
	if (args[0][0] == '-')
		args[0][2] = '\0';
	length = _strlen(name) + _strlen(str) + _strlen(args[0]) + 24;
	err = malloc(sizeof(char) * (length + 1));
	if (!err)
	{
		free(str);
		return (NULL);
	}
	_strcpy(err, name);
	_strcat(err, ": ");
	_strcat(err, str);
	if (args[0][0] == '-')
		_strcat(err, ": cd: Illegal option ");
	else
		_strcat(err, ": cd: can't cd to ");
	_strcat(err, args[0]);
	_strcat(err, "\n");
	free(str);
	return (err);
}
/**
 * error_2_exit - Generate an error message.
 * @args: An array of argument.
 *
 *
 * Return: Error message.
 */
char *error_2_exit(char **args)
{
	char *err, *str;
	int length;

	str = _itoa(hist);
	if (!str)
		return (NULL);
	length = _strlen(name) + _strlen(str) + _strlen(args[0]) + 27;
	err = malloc(sizeof(char) * (length + 1));
	if (!err)
	{
		free(str);
		return (NULL);
	}
	_strcpy(err, name);
	_strcat(err, ": ");
	_strcat(err, str);
	_strcat(err, ": exit: Illegal number: ");
	_strcat(err, args[0]);
	_strcat(err, "\n");
	free(str);
	return (err);
}

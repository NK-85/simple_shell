#include "shell.h"

char *error_127(char **args);
char *error_126(char **args);

/**
 * error_127 - Generates an error message.
 * @args: An array of argument.
 *
 * Return: An error
 */
char *error_127(char **args)
{
	char *err, *str;
	int length;

	str = _itoa(hist);
	if (!str)
		return (NULL);
	length = _strlen(name) + _strlen(str) + _strlen(args[0]) + 16;
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
	_strcat(err, ": not found\n");
	free(str);
	return (err);
}

/**
 * error_126 - Created an error message.
 * @args: An array of arguments.
 *
 *
 * Return: Error.
 */
char *error_126(char **args)
{
	char *err, *str;
	int length;

	str = _itoa(hist);
	if (!str)
		return (NULL);
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
	_strcat(err, ": ");
	_strcat(err, args[0]);
	_strcat(err, ": Permission denied\n");
	free(str);
	return (err);
}

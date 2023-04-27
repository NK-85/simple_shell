#include "shell.h"
char *_itoa(int num);
int num_len(int num);
int create_error(char **args, int err);
/**
 * num_len - Counts the digit length of a number.
 * @num: The number to measure.
 *
 * Return: The digit length.
 */
int num_len(int num)
{
	unsigned int number1;
	int length = 1;

	if (num < 0)
	{
		length++;
		number1 = num * -1;
	}
	else
	{
		number1 = num;
	}
	while (number1 > 9)
	{
		length++;
		number1 /= 10;
	}
	return (length);
}

/**
 * _itoa - Changesinteger to a string.
 * @num: Integer.
 *
 * Return: Strings converted.
 */
char *_itoa(int num)
{
	char *buffer;
	int length = num_len(num);
	unsigned int number1;

	buffer = malloc(sizeof(char) * (length + 1));
	if (!buffer)
		return (NULL);

	buffer[length] = '\0';

	if (num < 0)
	{
		number1 = num * -1;
		buffer[0] = '-';
	}
	else
	{
		number1 = num;
	}

	length--;
	do {
		buffer[length] = (number1 % 10) + '0';
		number1 /= 10;
		length--;
	} while (number1 > 0);

	return (buffer);
}

/**
 * create_error - Writing a custom error message to stderr.
 * @args: An array of arguments.
 * @err: The error value.
 *
 * Return: The error value.
 */
int create_error(char **args, int err)
{
	char *Errors;

	switch (err)
	{
	case -1:
		Errors = error_env(args);
		break;
	case 1:
		Errors = error_1(args);
		break;
	case 2:
		if (*(args[0]) == 'e')
			Errors = error_2_exit(++args);
		else if (args[0][0] == ';' || args[0][0] == '&' || args[0][0] == '|')
			Errors = error_2_syntax(args);
		else
			Errors = error_2_cd(args);
		break;
	case 126:
		Errors = error_126(args);
		break;
	case 127:
		Errors = error_127(args);
		break;
	}
	write(STDERR_FILENO, Errors, _strlen(Errors));
	if (Errors)
		free(Errors);
	return (err);
}

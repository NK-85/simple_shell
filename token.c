#include "shell.h"

int token_len(char *str, char *delim);
int count_tokens(char *str, char *delim);
char **_strtok(char *line, char *delim);

/**
 * _strtok - Tokenize str.
 * @line: The str.
 * @delim: The character,delim to tokenize the string by.
 *
 * Return: An array containing the tokenized string.
 */
char **_strtok(char *line, char *delim)
{
	char **ptx;
	int i = 0, token, j, lett, l;

	token = count_tokens(line, delim);
	if (token == 0)
		return (NULL);
	ptx = malloc(sizeof(char *) * (token + 2));
	if (!ptx)
		return (NULL);
	for (j = 0; j < token; j++)
	{
		while (line[i] == *delim)
			i++;
		lett = token_len(line + i, delim);
		ptx[j] = malloc(sizeof(char) * (lett + 1));
		if (!ptx[j])
		{
			for (i -= 1; i >= 0; i--)
				free(ptx[i]);
			free(ptx);
			return (NULL);
		}
		for (l = 0; l < lett; l++)
		{
			ptx[j][l] = line[i];
			i++;
		}
		ptx[j][l] = '\0';
	}
	ptx[j] = NULL;
	ptx[j + 1] = NULL;

	return (ptx);
}

/**
 * token_len - Locates delimiter index within a string.
 * @str: The str that will be searched.
 * @delim: The delimiter character.
 *
 * Return: The
 */
int token_len(char *str, char *delim)
{
	int i = 0, length = 0;

	while (*(str + i) && *(str + i) != *delim)
	{
		length++;
		i++;
	}
	return (length);
}

/**
 * count_tokens - Counts the number of string.
 * @str: String searched.
 * @delim: The delim character.
 *
 * Return: The no of token
 */
int count_tokens(char *str, char *delim)
{
	int i, tokn = 0, length = 0;

	for (i = 0; *(str + i); i++)
		length++;
	for (i = 0; i < length; i++)
	{
		if (*(str + i) != *delim)
		{
			tokn++;
			i += token_len(str + i, delim);
		}
	}
	return (tokn);
}

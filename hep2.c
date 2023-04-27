#include "shell.h"
void logical_ops(char *line, ssize_t *new_len);
ssize_t get_new_len(char *line);
void handle_line(char **line, ssize_t read);

/**
 * handle_line - Splits a line read from standard input.
 * @line: A line pointer read from standard input.
 * @read: The length of line.
 *
 * Description: Insert space to separate ";", "||", and "&&".
 * and replace "#" with '\0'.
 */
void handle_line(char **line, ssize_t read)
{
	char *oline, *nline;
	char prev, curr, nxt;
	size_t i, j;
	ssize_t new_length;

	new_length = get_new_len(*line);
	if (new_length == read - 1)
		return;
	nline = malloc(new_length + 1);
	if (!nline)
		return;
	j = 0;
	oline = *line;
	for (i = 0; oline[i]; i++)
	{
		curr = oline[i];
		nxt = oline[i + 1];
		if (i != 0)
		{
			prev = oline[i - 1];
			if (curr == ';')
			{
				if (nxt == ';' && prev != ' ' && prev != ';')
				{
					nline[j++] = ' ';
					nline[j++] = ';';
					continue;
				}
				else if (prev == ';' && nxt != ' ')
				{
					nline[j++] = ';';
					nline[j++] = ' ';
					continue;
				}
				if (prev != ' ')
					nline[j++] = ' ';
				nline[j++] = ';';
				if (nxt != ' ')
					nline[j++] = ' ';
				continue;
			}
			else if (curr == '&')
			{
				if (nxt == '&' && prev != ' ')
					nline[j++] = ' ';
				else if (prev == '&' && nxt != ' ')
				{
					nline[j++] = '&';
					nline[j++] = ' ';
					continue;
				}
			}
			else if (curr == '|')
			{
				if (nxt == '|' && prev != ' ')
					nline[j++]  = ' ';
				else if (prev == '|' && nxt != ' ')
				{
					nline[j++] = '|';
					nline[j++] = ' ';
					continue;
				}
			}
		}
		else if (curr == ';')
		{
			if (i != 0 && oline[i - 1] != ' ')
				nline[j++] = ' ';
			nline[j++] = ';';
			if (nxt != ' ' && nxt != ';')
				nline[j++] = ' ';
			continue;
		}
		nline[j++] = oline[i];
	}
	nline[j] = '\0';

	free(*line);
	*line = nline;
}

/**
 * get_new_len - Recieves the new leng of a line partitioned
 *by ";", "||", "&&&", or "#".
 * @line: The line to check through the code.
 *
 * Return: The new length of the line.
 *
 */

ssize_t get_new_len(char *line)
{
	size_t j;
	ssize_t nlen = 0;
	char curr, nxt;

	for (j = 0; line[j]; j++)
	{
		curr = line[j];
		nxt = line[j + 1];
		if (curr == '#')
		{
			if (j == 0 || line[j - 1] == ' ')
			{
				line[j] = '\0';
				break;
			}
		}
		else if (j != 0)
		{
			if (curr == ';')
			{
				if (nxt == ';' && line[j - 1] != ' ' && line[j - 1] != ';')
				{
					nlen += 2;
					continue;
				}
				else if (line[j - 1] == ';' && nxt != ' ')
				{
					nlen += 2;
					continue;
				}
				if (line[j - 1] != ' ')
					nlen++;
				if (nxt != ' ')
					nlen++;
			}
			else
				logical_ops(&line[j], &nlen);
		}
		else if (curr == ';')
		{
			if (j != 0 && line[j - 1] != ' ')
				nlen++;
			if (nxt != ' ' && nxt != ';')
				nlen++;
		}
		nlen++;
	}
	return (nlen);
}
/**
 * logical_ops - Confirm a logical operators "||" or "&&".
 * @line: Character pointer to check in the line.
 * @new_len: New_len pointer in get_new_len function.
 *
 */
void logical_ops(char *line, ssize_t *new_len)
{
	char prev, nxt, curr;

	prev = *(line - 1);
	curr = *line;
	nxt = *(line + 1);

	if (curr == '&')
	{
		if (nxt == '&' && prev != ' ')
			(*new_len)++;
		else if (prev == '&' && nxt != ' ')
			(*new_len)++;
	}
	else if (curr == '|')
	{
		if (nxt == '|' && prev != ' ')
			(*new_len)++;
		else if (prev == '|' && nxt != ' ')
			(*new_len)++;
	}
}


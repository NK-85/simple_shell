#include "shell.h"

ssize_t _getline(char **lineptr, size_t *n, FILE *stream);
void assign_lineptr(char **lineptr, size_t *n, char *buffer, size_t b);
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size);


/**
 * _realloc - To reallocates a memory block using free and malloc.
 * @ptr: A pointer to the memory.
 * @old_size: The size befor for old memory.
 * @new_size: The size in bytes for the new memory block.
 *
 * Return: Otherwise - a pointer to the reallocated memory block.
 */
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
	void *memory;
	char *cptr, *mem;
	unsigned int i;

	if (new_size == old_size)
		return (ptr);

	if (ptr == NULL)
	{
		memory = malloc(new_size);
		if (memory == NULL)
			return (NULL);

		return (memory);
	}

	if (new_size == 0 && ptr != NULL)
	{
		free(ptr);
		return (NULL);
	}

	cptr = ptr;
	memory = malloc(sizeof(*cptr) * new_size);
	if (memory == NULL)
	{
		free(ptr);
		return (NULL);
	}

	mem = memory;

	for (i = 0; i < old_size && i < new_size; i++)
		mem[i] = *cptr++;

	free(ptr);
	return (memory);
}

/**
 * assign_lineptr - Reassigns the lineptr variable for _getline.
 * @lineptr: A buffer to store an input string.
 * @n: The size of lineptr.
 * @b: The size of buffer
 * @buffer: The string to assign to lineptr.
 */
void assign_lineptr(char **lineptr, size_t *n, char *buffer, size_t b)
{
	if (*lineptr == NULL)
	{
		if (b > 120)
			*n = b;
		else
			*n = 120;
		*lineptr = buffer;
	}
	else if (*n < b)
	{
		if (b > 120)
			*n = b;
		else
			*n = 120;
		*lineptr = buffer;
	}
	else
	{
		_strcpy(*lineptr, buffer);
		free(buffer);
	}
}

/**
 * _getline - Get input from a stream.
 * @lineptr: Store the input to buffer.
 * @n: The size of the lineptr.
 * @stream: The stream to read from.
 *
 * Return: Number of bytes to read.
 */
ssize_t _getline(char **lineptr, size_t *n, FILE *stream)
{
	static ssize_t a;
	ssize_t ret;
	char c = 'x', *buffer;
	int r;

	if (a == 0)
		fflush(stream);
	else
		return (-1);
	a = 0;

	buffer = malloc(sizeof(char) * 120);
	if (!buffer)
		return (-1);

	while (c != '\n')
	{
		r = read(STDIN_FILENO, &c, 1);
		if (r == -1 || (r == 0 && a == 0))
		{
			free(buffer);
			return (-1);
		}
		if (r == 0 && a != 0)
		{
			a++;
			break;
		}
		if (a >= 120)
			buffer = _realloc(buffer, a, a + 1);

		buffer[a] = c;
		a++;
	}
	buffer[a] = '\0';

	assign_lineptr(lineptr, n, buffer, a);

	ret = a;
	if (r != 0)
		a = 0;
	return (ret);
}

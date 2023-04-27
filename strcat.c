#include "shell.h"
char *_strcat(char *dest, const char *src);
char *_strncat(char *dest, const char *src, size_t n);
int _strlen(const char *s);
char *_strcpy(char *dest, const char *src);

/**
 * _strcat - Concantenates two strings.
 * @dest: Pointer to destination string.
 * @src: Pointer to source string.
 *
 * Return: Pointer to destination string.
 */
char *_strcat(char *dest, const char *src)
{
	char *destination;
	const char *source;

	destination = dest;
	source =  src;

	while (*destination != '\0')
		destination++;

	while (*source != '\0')
		*destination++ = *source++;
	*destination = '\0';
	return (dest);
}

/**
 * _strcpy - Copies the string.
 *
 * @dest: The destination of copied string.
 * @src: The source string to be copied from.
 *
 * Return: copied strings to dest.
 */
char *_strcpy(char *dest, const char *src)
{
	size_t k;

	for (k = 0; src[k] != '\0'; k++)
		dest[k] = src[k];
	dest[k] = '\0';
	return (dest);
}

/**
 * _strncat - Concantenates two strings
 *
 * @dest: Destination string.
 * @src: Source string.
 * @n: Number of bytes to copy.
 *
 * Return: Pointer to destination string.
 */
char *_strncat(char *dest, const char *src, size_t n)
{
	size_t length_dest = _strlen(dest);
	size_t j;

	for (j = 0; j < n && src[j] != '\0'; j++)
		dest[length_dest + j] = src[j];
	dest[length_dest + j] = '\0';

	return (dest);
}
/**
 * _strlen - Returns the length of a string.
 * @s: String.
 *
 * Return: Length of string.
 */
int _strlen(const char *s)
{
	int len = 0;

	if (!s)
		return (len);
	for (len = 0; s[len]; len++)
		;
	return (len);
}

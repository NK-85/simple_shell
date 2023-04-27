#include "shell.h"
int _strspn(char *s, char *accept);
char *_strchr(char *s, char c);
int _strncmp(const char *s1, const char *s2, size_t n);
int _strcmp(char *s1, char *s2);

/**
 * _strchr - Character  to be located in a string.
 * @s: The string.
 * @c: The character.
 *
 * Return: If c is found - a pointer to the first occurence.
 */
char *_strchr(char *s, char c)
{
	int i;

	for (i = 0; s[i]; i++)
	{
		if (s[i] == c)
			return (s + i);
	}
	return (NULL);
}

/**
 * _strspn - Length of a substring.
 * @s: String.
 * @accept: Measured.
 *
 * Return: The number of bytes in s which
 */

int _strspn(char *s, char *accept)
{
	int no_bytes = 0;
	int i;

	while (*s)
	{
		for (i = 0; accept[i]; i++)
		{
			if (*s == accept[i])
			{
				no_bytes++;
				break;
			}
		}
		s++;
	}
	return (no_bytes);
}

/**
 * _strcmp - Compares two strings.
 * @s1: The first string.
 * @s2: The second string.
 *
 * Return: return s1-s2
 */
int _strcmp(char *s1, char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	if (*s1 != *s2)
		return (*s1 - *s2);
	return (0);
}

/**
 * _strncmp - Compare two strings.
 * @s1: String pointer.
 * @s2: String pointer.
 * @n: The no of bytes used to compare strings.
 *
 * Return: |s1 - s2|.
 */
int _strncmp(const char *s1, const char *s2, size_t n)
{
	size_t index;

	for (index = 0; s1[index] && s2[index] && index < n; index++)
	{
		if (s1[index] > s2[index])
			return (s1[index] - s2[index]);
		else if (s1[index] < s2[index])
			return (s1[index] - s2[index]);
	}
	if (index == n)
		return (0);
	else
		return (-15);
}

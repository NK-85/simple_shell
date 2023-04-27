#include "shell.h"

void print_alias(alias_t *alias);
void set_alias(char *var_name, char *value);
int shellby_alias(char **args, char __attribute__((__unused__)) **front);

/**
 * shellby_alias - A command of Builtin that either prints all aliases, specific
 * aliases, or sets an alias.
 * @args: An array arguments.
 * @front: pointer to the beginning of args.
 *
 * Return: if an error occurs, return -1 else 0.
 */
int shellby_alias(char **args, char __attribute__((__unused__)) **front)
{
	alias_t *head = aliases;
	int i, retr = 0;
	char *mark;

	if (!args[0])
	{
		while (head)
		{
			print_alias(head);
			head = head->next;
		}
		return (retr);
	}
	for (i = 0; args[i]; i++)
	{
		head = aliases;
		mark = _strchr(args[i], '=');
		if (!mark)
		{
			while (head)
			{
				if (_strcmp(args[i], head->name) == 0)
				{
					print_alias(head);
					break;
				}
				head = head->next;
			}
			if (!head)
				retr = create_error(args + i, 1);
		}
		else
			set_alias(args[i], mark);
	}
	return (retr);
}

/**
 * set_alias - Sets an alias.
 * @var_name: Alias name.
 * @value: Alias
 */
void set_alias(char *var_name, char *value)
{
	alias_t *head = aliases;
	int length, j, i;
	char *mark2;

	*value = '\0';
	value++;
	length = _strlen(value) - _strspn(value, "'\"");
	mark2 = malloc(sizeof(char) * (length + 1));
	if (!mark2)
		return;
	for (i = 0, j = 0; value[i]; i++)
	{
		if (value[i] != '\'' && value[i] != '"')
			mark2[j++] = value[i];
	}
	mark2[j] = '\0';
	while (head)
	{
		if (_strcmp(var_name, head->name) == 0)
		{
			free(head->value);
			head->value = mark2;
			break;
		}
		head = head->next;
	}
	if (!head)
		add_alias_end(&aliases, var_name, mark2);
}

/**
 * print_alias - Prints alias in value format.
 * @alias: Alias pointer.
 */ 
void print_alias(alias_t *alias)
{
	char *alias_str;
	int length = _strlen(alias->name) + _strlen(alias->value) + 4;

	alias_str = malloc(sizeof(char) * (length + 1));
	if (!alias_str)
		return;
	_strcpy(alias_str, alias->name);
	_strcat(alias_str, "='");
	_strcat(alias_str, alias->value);
	_strcat(alias_str, "'\n");

	write(STDOUT_FILENO, alias_str, length);
	free(alias_str);
}
/**
 * replace_aliases - Replaces any matching alias with their value
 * whie going through the argument
 * @args: arguments pointer.
 *
 * Return: arguments pointer.
 */
char **replace_aliases(char **args)
{
	alias_t *head;
	int j;
	char *mark2;

	if (_strcmp(args[0], "alias") == 0)
		return (args);
	for (j = 0; args[j]; j++)
	{
		head = aliases;
		while (head)
		{
			if (_strcmp(args[j], head->name) == 0)
			{
				mark2 = malloc(sizeof(char) * (_strlen(head->value) + 1));
				if (!mark2)
				{
					free_args(args, args);
					return (NULL);
				}
				_strcpy(mark2, head->value);
				free(args[j]);
				args[j] = mark2;
				j--;
				break;
			}
			head = head->next;
		}
	}

	return (args);
}

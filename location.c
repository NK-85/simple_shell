#include "shell.h"
list_t *get_path_dir(char *path);
char *fill_path_dir(char *path);

/**
 * fill_path_dir - This Copies the path and replaces the trailing colon
 * @path: The list of directories(colomn seperated).
 *
 * Return: A copy of path with any trailing colons
 */
char *fill_path_dir(char *path)
{
	int w, L = 0;
	char *path_cp, *_pwd;

	_pwd = *(_getenv("PWD")) + 4;
	for (w = 0; path[w]; w++)
	{
		if (path[w] == ':')
		{
			if (path[w + 1] == ':' || w == 0 || path[w + 1] == '\0')
				L += _strlen(_pwd) + 1;
			else
				L++;
		}
		else
			L++;
	}
	path_cp = malloc(sizeof(char) * (L + 1));
	if (!path_cp)
		return (NULL);
	path_cp[0] = '\0';
	for (w = 0; path[w]; w++)
	{
		if (path[w] == ':')
		{
			if (w == 0)
			{
				_strcat(path_cp, _pwd);
				_strcat(path_cp, ":");
			}
			else if (path[w + 1] == ':' || path[w + 1] == '\0')
			{
				_strcat(path_cp, ":");
				_strcat(path_cp, _pwd);
			}
			else
				_strcat(path_cp, ":");
		}
		else
		{
			_strncat(path_cp, &path[w], 1);
		}
	}
	return (path_cp);
}
/**
 * get_location - Locating a command in the PATH.
 * @command: The command to be located.
 *
 * Return: If command cannot be located-NULL
 */
char *get_location(char *command)
{
	char **paths, *tmp;
	list_t *dirs, *link;
	struct stat st;

	paths = _getenv("PATH");
	if (!paths || !(*paths))
		return (NULL);

	dirs = get_path_dir(*paths + 5);
	link = dirs;

	while (dirs)
	{
		tmp = malloc(_strlen(dirs->dir) + _strlen(command) + 2);
		if (!tmp)
			return (NULL);

		_strcpy(tmp, dirs->dir);
		_strcat(tmp, "/");
		_strcat(tmp, command);
		if (stat(tmp, &st) == 0)
		{
			free_list(link);
			return (tmp);
		}

		dirs = dirs->next;
		free(tmp);
	}

	free_list(link);

	return (NULL);
}

/**
 * get_path_dir - Tokenizes list of directories into a
 * linked list.
 * @path: The list of directories.
 *
 * Return: A pointer to initialized linked list.
 */
list_t *get_path_dir(char *path)
{
	int i;
	char **dirs, *path_cp;
	list_t *link = NULL;

	path_cp = fill_path_dir(path);
	if (!path_cp)
		return (NULL);
	dirs = _strtok(path_cp, ":");
	free(path_cp);
	if (!dirs)
		return (NULL);
	for (i = 0; dirs[i]; i++)
	{
		if (add_node_end(&link, dirs[i]) == NULL)
		{
			free_list(link);
			free(dirs);
			return (NULL);
		}
	}

	free(dirs);
	return (link);
}


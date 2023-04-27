#include "shell.h"

void free_env(void);
char **_copyenv(void);
char **_getenv(const char *var);

/**
 * free_env - Copy and free the environment.
 *
 */
void free_env(void)
{
	int i;

	for (i = 0; environ[i]; i++)
		free(environ[i]);
	free(environ);
}

/**
 * _getenv - Get E-var from the PATH.
 * @var: Name of E-var.
 *
 * Return: Execute null if the E-var doesnt exist.
 */
char **_getenv(const char *var)
{
	int length, i;

	length = _strlen(var);
	for (i = 0; environ[i]; i++)
	{
		if (_strncmp(var, environ[i], length) == 0)
			return (&environ[i]);
	}

	return (NULL);
}
char **_getenv(const char *var);

/**
 * _copyenv - Creating a copy of the envir.
 *
 * Return: Error occurs, return NULL.
 */
char **_copyenv(void)
{
        char **pp;
        size_t n;
        int i;

        for (n = 0; environ[n]; n++)
                ;

        pp = malloc(sizeof(char *) * (n + 1));
        if (!pp)
                return (NULL);

        for (i = 0; environ[i]; i++)
        {
                pp[i] = malloc(_strlen(environ[i]) + 1);

                if (!pp[i])
                {
                        for (i--; i >= 0; i--)
                                free(pp[i]);
                        free(pp);
                        return (NULL);
                }
                _strcpy(pp[i], environ[i]);
        }
        pp[i] = NULL;
	return (pp);
}

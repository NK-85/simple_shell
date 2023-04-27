#include "shell.h"

void help_history(void);
void help_setenv(void);
void help_env(void);
void help_unsetenv(void);

/**
 * help_setenv - Displays the infor 'setenv' on the shellby builtin command.
 *
 */
void help_setenv(void)
{
	char *message = "setenv: setenv [VARIABLE] [VALUE]\n\tInitializes a new";

	write(STDOUT_FILENO, message, _strlen(message));
	message = "environment variable, or modifies an existing one.\n\n";
	write(STDOUT_FILENO, message, _strlen(message));
	message = "\tUpon failure, prints a message to stderr.\n";
	write(STDOUT_FILENO, message, _strlen(message));
}

/**
 * help_env - Displays info 'env' on the builtin command.
 *
 */
void help_env(void)
{
	char *message = "env: env\n\tPrints the current environment.\n";

	write(STDOUT_FILENO, message, _strlen(message));
}

/**
 * help_unsetenv - Displays the info'unsetenv' on the builtin command
 *
 */
void help_unsetenv(void)
{
	char *message = "unsetenv: unsetenv [VARIABLE]\n\tRemoves an ";

	write(STDOUT_FILENO, message, _strlen(message));
	message = "environmental variable.\n\n\tUpon failure, prints a ";
	write(STDOUT_FILENO, message, _strlen(message));
	message = "message to stderr.\n";
	write(STDOUT_FILENO, message, _strlen(message));
}

#include "shell.h"

int execute(char **args, char **front);
void sig_handler(int sig);

/**
 * sig_handler - This prints a new prompt given a signal.
 * @sig: The signal of the prompt.
 */
void sig_handler(int sig)
{
	char *propt = "\n#cisfun$ ";

	(void)sig;
	signal(SIGINT, sig_handler);
	write(STDIN_FILENO, propt, 10);
}

/**
 * execute - A command is executed in a child process.
 * @args: An array of arguments.
 * @front: A pointer to the beginning of args.
 *
 * Return: If an error occur exit value of the last executed command.
 */
int execute(char **args, char **front)
{
	pid_t child_pid;
	int retr = 0, status, temp = 0;
	char *comm = args[0];

	if (comm[0] != '/' && comm[0] != '.')
	{
		temp = 1;
		comm = get_location(comm);
	}

	if (!comm || (access(comm, F_OK) == -1))
	{
		if (errno == EACCES)
			retr = (create_error(args, 126));
		else
			retr = (create_error(args, 127));
	}
	else
	{
		child_pid = fork();
		if (child_pid == -1)
		{
			if (temp)
				free(comm);
			perror("Error child:");
			return (1);
		}
		if (child_pid == 0)
		{
			execve(comm, args, environ);
			if (errno == EACCES)
				retr = (create_error(args, 126));
			free_env();
			free_args(args, front);
			free_alias_list(aliases);
			_exit(retr);
		}
		else
		{
			wait(&status);
			retr = WEXITSTATUS(status);
		}
	}
	if (temp)
		free(comm);
	return (retr);
}

/**
 * main - A simple UNIX command interpreter is been run.
 * @argc: The count of arguments.
 * @argv: An array of arguments pointer.
 *
 * Return: Value of the  command executed.
 */
int main(int argc, char *argv[])
{
	int retn, retr = 0;
	int *exe_ret = &retn;
	char *propt = "#cisfun$ ", *new_line = "\n";

	name = argv[0];
	hist = 1;
	aliases = NULL;
	signal(SIGINT, sig_handler);

	*exe_ret = 0;
	environ = _copyenv();
	if (!environ)
		exit(-100);

	if (argc != 1)
	{
		retr = proc_file_commands(argv[1], exe_ret);
		free_env();
		free_alias_list(aliases);
		return (*exe_ret);
	}

	if (!isatty(STDIN_FILENO))
	{
		while (retr != END_OF_FILE && retr != EXIT)
			retr = handle_args(exe_ret);
		free_env();
		free_alias_list(aliases);
		return (*exe_ret);
	}

	while (1)
	{
		write(STDOUT_FILENO, propt, 10);
		retr = handle_args(exe_ret);
		if (retr == END_OF_FILE || retr == EXIT)
		{
			if (retr == END_OF_FILE)
				write(STDOUT_FILENO, new_line, 10);
			free_env();
			free_alias_list(aliases);
			exit(*exe_ret);
		}
	}

	free_env();
	free_alias_list(aliases);
	return (*exe_ret);
}

#include "shell.h"

char *get_args(char *line, int *exe_ret);
int run_args(char **args, char **front, int *exe_ret);
int handle_args(int *exe_ret);
int check_args(char **args);
int call_args(char **args, char **front, int *exe_ret);

/**
 * get_args - Recieve a command from standard input.
 * @line: A buffeliner to store the command.
 *
 * @exe_ret: The value returned.
 *
 * Return: NULL if an error occurs
 */
char *get_args(char *line, int *exe_ret)
{
        size_t size = 0;
        char *prompt = "$ ";
        ssize_t Read;

        if (line)
                free(line);
        Read = _getline(&line, &size, STDIN_FILENO);
        if (Read == -1)
                return (NULL);
        if (Read == 1)
        {
                hist++;
                if (isatty(STDIN_FILENO))
                        write(STDOUT_FILENO, prompt, 2);
                return (get_args(line, exe_ret));
        }

        line[Read - 1] = '\0';
        variable_replacement(&line, exe_ret);
        handle_line(&line, Read);

        return (line);
}
/**
 * check_args - Checks if there are  arguments ','; ';;', '&&', or '||'.
 * @args: A  pointer tokenized commands and arguments.
 *
 * Return: return -2 if it takes an argument else 0.
 */
int check_args(char **args)
{
        size_t index;
        char *vik, *lif;

        for (index = 0; args[index]; index++)
        {
                vik = args[index];
                if (vik[0] == ';' || vik[0] == '&' || vik[0] == '|')
                {
                        if (index == 0 || vik[1] == ';')
                                return (create_error(&args[index], 2));
                        lif = args[index + 1];
                        if (lif && (lif[0] == ';' || lif[0] == '&' || lif[0] == '|'))
                                return (create_error(&args[index + 1], 2));
                }
        }
        return (0);
}

/**
 * call_args - Spliting operators.
 * @args: An array of arguments.
 * @front: A pointer to args.
 * @exe_ret: The return value of the parent process' last executed command.
 *
 *
 * Return: The value of the last executed command.
 */
int call_args(char **args, char **front, int *exe_ret)
{
        int i, Ret;

        if (!args[0])
                return (*exe_ret);
        for (i = 0; args[i]; i++)
        {
                if (_strncmp(args[i], "||", 2) == 0)
                {
                        free(args[i]);
                        args[i] = NULL;
                        args = replace_aliases(args);
                        Ret = run_args(args, front, exe_ret);
                        if (*exe_ret != 0)
                        {
                                args = &args[++i];
                                i = 0;
                        }
                        else
                        {
                                for (i++; args[i]; i++)
                                        free(args[i]);
                                return (Ret);
                        }
                }
                else if (_strncmp(args[i], "&&", 2) == 0)
                {
                        free(args[i]);
                        args[i] = NULL;
                        args = replace_aliases(args);
                        Ret = run_args(args, front, exe_ret);
                        if (*exe_ret == 0)
                        {
                                args = &args[++i];
                                i = 0;
                        }
                        else
                        {
                                for (i++; args[i]; i++)
                                        free(args[i]);
                                return (Ret);
                        }
                }
        }
        args = replace_aliases(args);
        Ret = run_args(args, front, exe_ret);
        return (Ret);
}

/**
 * run_args - Callings to execute a command.
 * @args: An array of arguments.
 * @front: A pointerto the args.
 * @exe_ret: It returns the value parent process'.
 *
 *
 * Return: The return value of the l. exec command.
 */
int run_args(char **args, char **front, int *exe_ret)
{
        int Ret, index;
        int (*builtin)(char **args, char **front);

        builtin = get_builtin(args[0]);

        if (builtin)
        {
                Ret = builtin(args + 1, front);
                if (Ret != EXIT)
                        *exe_ret = Ret;
        }
        else
        {
                *exe_ret = execute(args, front);
                Ret = *exe_ret;
        }
        hist++;
        for (index = 0; args[index]; index++)
                free(args[index]);
        return (Ret);
}

/**
 * handle_args - Call,get and runs the execution of a command.
 * @exe_ret: value of the parent process' last executed command.
 *
 * Return: execur return
 */
int handle_args(int *exe_ret)
{
        int j, ret = 0;
        char **arg, *line = NULL, **f;

        line = get_args(line, exe_ret);
        if (!line)
                return (END_OF_FILE);
        arg = _strtok(line, " ");
        free(line);
        if (!arg)
                return (ret);
        if (check_args(arg) != 0)
        {
                *exe_ret = 2;
                free_args(arg, arg);
                return (*exe_ret);
        }
        f = arg;

        for (j = 0; arg[j]; j++)
        {
                if (_strncmp(arg[j], ";", 1) == 0)
                {
                        free(arg[j]);
                        arg[j] = NULL;
                        ret = call_args(arg, f, exe_ret);
                        arg = &arg[++j];
                        j = 0;
                }
        }
        if (arg)
                ret = call_args(arg, f, exe_ret);

        free(f);
        return (ret);
}

Hep2.c
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

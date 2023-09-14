#include "shell.h"

int status;
char *_shellname;

/**
 * _cmdmanager - manages the process a command goes through to get executed
 * @args: command and arguments
 *
 * Return: TRUE if success, FALSE if failure
 */
int _cmdmanager(char **args)
{
	char **args_ptr = args;
	int prev_eval = NEITHER, no_err = TRUE, whichcmd;
	char prev_op = 'c', next_op = 'c';

	while (*args != NULL && prev_eval != EXIT_SH)
	{
		while (*args_ptr != NULL && **args_ptr != '&'
			   && **args_ptr != '|')
			args_ptr++;
		if (_strcmp(*args_ptr, "||", MATCH) == TRUE)
		{
			*args_ptr = NULL;
			args_ptr++;
			next_op = '|';
		}
		if (_strcmp(*args_ptr, "&&", MATCH) == TRUE)
		{
			*args_ptr = NULL;
			args_ptr++;
			next_op = '&';
		}
		if (next_op == 'c')
			break;
		prev_eval = and_or(args, prev_op, prev_eval);
		if (prev_eval == FALSE)
			no_err = FALSE;
		prev_op = next_op;
		args = args_ptr;
	}
	if (next_op == 'c')
	{
		whichcmd = _execmd(args);
		if (whichcmd == EXIT_SH)
			return (EXIT_SH);
	}
	if (no_err == FALSE || whichcmd == FALSE)
		return (FALSE);
	return (TRUE);
}

/**
 * _builtins - checks if a command is a built in
 * @args: command and arguments
 *
 * Return: SKIP if built in, EXECVE if not a built in, EXIT_SH if
 * exit shell, EXIT_SHELL_CODE if exiting with a particular code
 */
int _builtins(char **args)
{
	char **args_ptr = args;
	int i, status;

	while (*args_ptr != NULL)
	{
		if (**args_ptr == '#')
		{
			*args_ptr = NULL;
			break;
		}
		if (_strcmp("~", *args_ptr, MATCH) == TRUE
			&& _getarrelement(environ, "HOME=") != NULL)
		{
			*args_ptr = _strdup(_getarrelement
					(environ, "HOME=") + 5);
			continue;
		}
		if (_strcmp("~/", *args_ptr, PREFIX) == TRUE
			&& _getarrelement(environ, "HOME=") != NULL)
		{
			*args_ptr = _strconcat(_getarrelement
						   (environ, "HOME=")+ 5, *args_ptr + 1);
		}
		*args_ptr = _checkvars(*args_ptr);
		args_ptr++;
	}
	if (*args == NULL)
		return (SKIP);
	i = _aliasfunc(args, FALSE);
	if (i == EXECVE || i == SKIP)
		return (i);
	if (_strcmp("exit", *args, MATCH) == TRUE && args[1] != NULL)
	{
		status = _atoi(args[1]);
		if (status < 0)
		{
			status = 2;
			_errormsg(args[0], args[1]);
			return (SKIP);
		}
	}
	if (_strcmp("exit", *args, MATCH) == TRUE)
		return (_myexit(status));
	else if (_strcmp("setenv", *args, MATCH) == TRUE && args[1] != NULL)
		return (_setenv(args[1], args[2]));
	else if (_strcmp("unsetenv", *args, MATCH) == TRUE
		 && args[1] != NULL)
		return (_unsetenv(args[1]));
	else if (_strcmp("cd", *args, MATCH) == TRUE)
		return (_cd(args[1]));
	else if (_strcmp("env", *args, MATCH) == TRUE)
		return (_printenv());
	return (-1);
}

/**
 * and_or - deals with command line logical operators
 * @args: command and arguments
 * @operator: first char of logical operator
 * @last_compare: if last command in logic evaluated to true or false
 *
 * Return: if this command evaluates to true or false
 */
int and_or(char **args, char operator, int last_compare)
{
	int i;

	if (last_compare == NEITHER)
	{
		i = _execmd(args);
		if (i == EXIT_SH)
			return (EXIT_SH);
		if (i == TRUE)
			return (TRUE);
		return (FALSE);
	}
	if (last_compare == TRUE && operator == '&')
	{
		i = _execmd(args);
		if (i == EXIT_SH)
			return (EXIT_SH);
		if (i == TRUE)
			return (TRUE);
		return (FALSE);
	}
	if (last_compare == FALSE && operator == '|')
	{
		i = _execmd(args);
		if (i == EXIT_SH)
			return (EXIT_SH);
		if (i == TRUE)
			return (TRUE);
		return (FALSE);
	}
	if (last_compare == TRUE && operator == '|')
		return (TRUE);
	return (FALSE);
}

/**
 * _checkcmd - checks if a non-built-in exists
 * @args: argument and commands
 *
 * Return: TRUE if valid command, FALSE if not
 */
char *_checkcmd(char **args)
{
	char *cmd_buf, *full_buf, *path_str = NULL, *path_ptr, *path_tmp;
	char **path_var = NULL, **path_var_ptr;

	if (access(*args, X_OK) == 0)
		return (_strdup(*args));
	if (_getarrelement(environ, "PATH=") != NULL)
		path_str = _strdup(_getarrelement(environ, "PATH=") + 5);
	path_ptr = path_str;
	if (path_str != NULL)
	{
		if (*path_str == ':')
		{
			cmd_buf = _strconcat("./", *args);
			if (access(cmd_buf, X_OK) == 0)
			{
				free(path_str);
				return (cmd_buf);
			}
			else
			{
				free(cmd_buf);
				path_ptr = _strdup(path_str + 1);
				free(path_str);
				path_str = path_ptr;
			}
		}
		while (*path_ptr != '\0')
		{
			if (*path_ptr == ':' && *(path_ptr + 1) == ':')
			{
				*(path_ptr + 1) = '\0';
				path_tmp = (_strconcat(path_str, ".:"));
				path_ptr = _strconcat(path_tmp, path_ptr + 2);
				free(path_str);
				free(path_tmp);
				path_str = path_ptr;
			}
			path_ptr++;
		}
	}
	if (path_str != NULL)
		path_var = _makearray(path_str, ':', NULL);
	path_var_ptr = path_var;
	cmd_buf = _strconcat("/", *args);
	full_buf = _strdup(cmd_buf);
	if (path_var != NULL)
	{
		while (*path_var_ptr != NULL && access(full_buf, X_OK) != 0)
		{
			free(full_buf);
			full_buf = _strconcat(*path_var_ptr, cmd_buf);
			path_var_ptr++;
		}
	}
	free(cmd_buf);
	free(path_str);
	free(path_var);
	if (access(full_buf, X_OK) != 0)
	{
		_errormsg(args[0], NULL);
		free(full_buf);
		return (NULL);
	}
	return (full_buf);
}

/**
 * _execmd - executes a command
 * @args: command and arguments
 *
 * Return: TRUE or EXIT_SH
 */
int _execmd(char **args)
{
	pid_t pid;
	char *buf_ptr = *args;
	char *cmd_name;
	int whichcmd = _builtins(args);

	if (whichcmd == EXECVE)
	{
		cmd_name = _checkcmd(args);
		if (cmd_name == NULL)
			return (FALSE);
		pid = fork();
		if (pid == 0)
		{
			execve(cmd_name, args, environ);
			perror("Error: ");
		}
		else if (pid < 0)
		{
			perror("Error: ");
		}
		wait(&status);
		free(cmd_name);
		fflush(stdin);
		if (status != 0)
			status = 2;
	}
	if (_strcmp("false", *args, MATCH) == TRUE)
		status = 1;
	if (*args != buf_ptr)
		free(*args);
	args++;

	while (*args != NULL)
	{
		while (*buf_ptr != '\0')
		{
			buf_ptr++;
		}
		buf_ptr++;

		if (*args != buf_ptr)
			free(*args);
		args++;
	}
	if (whichcmd == EXIT_SH)
		return (EXIT_SH);
	if (status != 0)
		return (FALSE);
	return (TRUE);
}

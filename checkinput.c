#include "shell.h"

/**
 * _saninput - sanitizes input from the command line
 * @old_buf: buffer to be sanitized
 * @old_size: size of old buffer
 *
 * Return: the new, sanitized buffer
 */
char *_saninput(char *old_buf, size_t *old_size)
{
	char *new_buf = malloc(*old_size * 3);
	char *new_ptr = new_buf;
	char *old_ptr = old_buf;

	while (*old_ptr != '\0')
	{
		while (*old_ptr == ' ')
			old_ptr++;
		while (*old_ptr	!= ' ' && *old_ptr != ';' && *old_ptr != '|'
			   && *old_ptr != '&' && *old_ptr != '\0')
		{
			*new_ptr = *old_ptr;
			new_ptr++;
			old_ptr++;
		}
		while (*old_ptr == ' ')
			old_ptr++;
		if (new_ptr != new_buf && *old_ptr != '\0')
		{
			*new_ptr = ' ';
			new_ptr++;
		}
		if (*old_ptr == ';' || *old_ptr == '|' || *old_ptr == '&')
		{
			if (_checkinput(old_ptr) == FALSE)
			{
				*old_size = 0;
				break;
			}
			*new_ptr = *old_ptr;
			if (*old_ptr == ';')
			{
				new_ptr++;
				*new_ptr = ' ';
			}
			else if (*(old_ptr + 1) == *old_ptr)
			{
				if (new_ptr == new_buf)
				{
					_errormsg(old_ptr, NULL);
					*old_size = 0;
					break;
				}
				new_ptr++;
				*new_ptr = *old_ptr;
				new_ptr++;
				*new_ptr = ' ';
				old_ptr++;
			}
			new_ptr++;
			old_ptr++;
		}
	}
	*new_ptr = '\0';
	free(old_buf);
	return (new_buf);
}

/**
 * _checkinput - helper function for sanitizer, check for unexpected char
 * @ptr: pointer to area that needs to be checked
 *
 * Return: TRUE if no error, FALSE if error
 */
int _checkinput(char *ptr)
{
	char *iter = ptr;

	iter++;
	if (*ptr == ';' && *iter == *ptr)
	{
		_errormsg(ptr, NULL);
		return (FALSE);
	}
	if (*iter == *ptr)
		iter++;

	while (*iter == ' ')
		iter++;

	if (*iter == ';' || *iter == '|' || *iter == '&')
	{
		_errormsg(iter, NULL);
		return (FALSE);
	}

	return (TRUE);
}

/**
 * _errormsg - prints error messages and sets status
 * @arg0: command that is causing error
 * @arg1: first argument to command
 */
void _errormsg(char *arg0, char *arg1)
{
	extern char *_shellname;
	extern int line_num;
	extern int status;
	char *err_str_num = _itoa(line_num);

	
	write(STDERR_FILENO, _shellname, _strlen(_shellname));
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, err_str_num, _strlen(err_str_num));
	free(err_str_num);

	if (_strcmp("cd", arg0, MATCH) == TRUE)
	{
		status = 2;
		write(STDERR_FILENO, ": cd: can't cd to ", 18);
		write(STDERR_FILENO, arg1, _strlen(arg1));
		write(STDERR_FILENO, "\n", 1);
		return;
	}

	if (_strcmp("exit", arg0, MATCH) == TRUE)
	{
		write(STDERR_FILENO, ": exit: Illegal number: ", 24);
		write(STDERR_FILENO, arg1, _strlen(arg1));
		write(STDERR_FILENO, "\n", 1);
		return;
	}
	if (*arg0 == ';' || *arg0 == '|' || *arg0 == '&')
	{
		status = 2;
		write(STDERR_FILENO, ": Syntax error: \"", 17);
		write(STDERR_FILENO, arg0, 1);
		if (*arg0 == *(arg0 + 1))
			write(STDERR_FILENO, arg0, 1);
		write(STDERR_FILENO, "\" unexpected\n", 14);
		return;
	}

	status = 127;
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, arg0, _strlen(arg0));
	write(STDERR_FILENO, ": not found\n", 12);
}

/**
 * _checkvars - checks an argument to see if it is a variable and if so,
 * replaces it with it's value
 * @arg: argument to be checked
 *
 * Return: a pointer to the evaluated argument (either new or an altered clone)
 */
char *_checkvars(char *arg)
{
	char *clone = NULL;
	char *ptr = arg;
	char *next, *tmp, *buffer;
	int is_var, i;

	extern char *_shellname;
	extern int status;

	while (*ptr != '\0')
	{
		if (*ptr == '$')
		{
			if (clone == NULL)
			{
				clone = _strdup(arg);
				i = ptr - arg;
				ptr = clone + i;
			}
			next = ptr + 1;
			while (*next != '\0' && *next != '#')
				next++;

			if (*next == '$' && next > ptr + 1)
				is_var = TRUE;
			else if (*next == '#')
				is_var = NEITHER;
			else
				is_var = FALSE;

			*next = '\0';

			if (_strcmp("$?", ptr, MATCH) == TRUE)
				tmp = _itoa(status);
			else if (_strcmp("$0", ptr, MATCH) == TRUE)
				tmp = _strdup(_shellname);
			else if (_strcmp("$$", ptr, MATCH) == TRUE)
				tmp = _itoa(getpid());
			else if (_getarrelement(environ, ptr + 1) != NULL)
			{
				buffer = _strconcat(ptr + 1, "=");
				tmp = _strdup(_getarrelement
				(environ, buffer) + _strlen(buffer));
				free(buffer);
			}
			else
				tmp = _strdup("");

			*ptr = '\0';
			ptr = _strconcat(clone, tmp);
			free(tmp);
			if (is_var == FALSE)
			{
				free(clone);
				clone = ptr;
				break;
			}
			if (is_var == TRUE)
				*next = '$';
			else if (is_var == NEITHER)
				*next = '#';
			tmp = _strconcat(ptr, next);
			free(ptr);
			ptr = tmp;
			free(clone);
			clone = ptr;
			if (is_var == NEITHER)
			{
				while (*ptr != '#')
					ptr++;
			}
		}
		ptr++;
	}
	if (clone != NULL)
		return (clone);

	return (arg);
}

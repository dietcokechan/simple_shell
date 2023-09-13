#include "shell.h"

/**
 * _freealias - frees all aliases
 * @alias_ptr: ptr to head of alias list
 *
 * Return: TRUE
 */
int _freealias(alias *alias_ptr)
{
	alias *tmp;

	while (alias_ptr != NULL)
	{
		tmp = alias_ptr;
		alias_ptr = alias_ptr->next;
		free(tmp->name);
		free(tmp->value);
		free(tmp);
	}

	return (TRUE);
}

/**
 * _checkalias - if the alias command is not called, this will check if the
 * command is an alias, and if so replace it with it's value
 * @args: argument to be checked
 * @alias_ptr: points to list of aliases to be checked against
 *
 * Return: TRUE
 */
int _checkalias(char **args, alias *alias_ptr)
{
	while (alias_ptr != NULL)
	{
		if (_strcmp(*args, alias_ptr->name, MATCH) == TRUE)
		{
			*args = _strdup(alias_ptr->value);
			return (-EXECVE);
		}
		alias_ptr = alias_ptr->next;
	}
	return (TRUE);
}

/**
 * _printalias - prints all aliases in list
 * @alias_ptr: points to list of aliases
 *
 * Return: SKIP
 */
int _printalias(alias *alias_ptr)
{
	while (alias_ptr != NULL)
	{
		write(STDOUT_FILENO, alias_ptr->name, _strlen(alias_ptr->name));
		write(STDOUT_FILENO, "=\'", 2);
		write(STDOUT_FILENO, alias_ptr->value,
			  _strlen(alias_ptr->value));
		write(STDOUT_FILENO, "\'\n", 2);
		alias_ptr = alias_ptr->next;
	}
	return (SKIP);
}

/**
 * _printaliasval - prints the value of a particular alias
 * @arg: name of alias
 * @alias_ptr: points to list of aliases
 *
 * Return: TRUE if valid alias, FALSE if not
 */
int _printaliasval(char *arg, alias *alias_ptr)
{
	int status;
	
	while (alias_ptr != NULL)
	{
		fflush(stdin);
		if (_strcmp(arg, alias_ptr->name, MATCH) == TRUE)
		{
			write(STDOUT_FILENO, arg, _strlen(arg));
			write(STDOUT_FILENO, "=\'", 2);
			write(STDOUT_FILENO, alias_ptr->value,
				  _strlen(alias_ptr->value));
			write(STDOUT_FILENO, "\'\n", 2);
			return (TRUE);
		}
		alias_ptr = alias_ptr->next;
	}

	status = 1;
	write(STDERR_FILENO, "alias: ", 7);
	write(STDERR_FILENO, arg, _strlen(arg));
	write(STDERR_FILENO, " not found\n", 11);

	return (FALSE);
}

/**
 * _setaliasval - initializes an alias or resets its value if it exists
 * @arg: name of alias
 * @alias_ptr: pointer to list of aliases
 * @new_value: value of alias to be set
 *
 * Return: TRUE
 */
int _setaliasval(char *arg, alias *alias_ptr, char *new_value)
{
	while (alias_ptr->next != NULL
		   && _strcmp(alias_ptr->name, arg, MATCH) != TRUE)
	{
		alias_ptr = alias_ptr->next;
	}

	if (_strcmp(alias_ptr->name, arg, MATCH) == TRUE)
	{
		free(alias_ptr->value);
	}
	else
	{
		alias_ptr->next = malloc(sizeof(alias *));
		alias_ptr = alias_ptr->next;
		if (alias_ptr == NULL)
			exit(EXIT_FAILURE);

		alias_ptr->name = _strdup(arg);
		alias_ptr->next = NULL;
	}
	alias_ptr->value = _strdup(new_value);

	return (TRUE);
}

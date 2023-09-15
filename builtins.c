#include "shell.h"

/**
 * _setenv - sets and environmental variable
 * @name: name of the variable
 * @value: value to set the variable to
 *
 * Return: 0 on success
 */
int _setenv(const char *name, const char *value)
{
	char **new_environ;
	char *buffer;
	char *buf_tmp;
	char *elem_ptr;
	int len;

	__attribute__((unused))shvars shvars;

	if (value == NULL)
	{
		write(STDERR_FILENO, "setenv: no value given\n", 23);
		shvars.status = 2;
		return (SKIP);
	}

	buffer = _strconcat((char *)name, "=");

	elem_ptr = _getarrelement(environ, buffer);

	buf_tmp = _strconcat(buffer, (char *)value);
	free(buffer);
	buffer = buf_tmp;

	if (elem_ptr == NULL)
	{
		len = _listlen(environ, NULL);
		new_environ = _arrcpy(environ, len + 1);
		new_environ[len - 1] = buffer;
		new_environ[len] = NULL;
		_freearr(environ);
		environ = new_environ;
		return (SKIP);
	}

	len = _listlen(environ, (char *)name);
	free(environ[len]);
	environ[len] = buffer;

	shvars.status = 0;

	return (SKIP);
}

/**
 * _unsetenv - deletes an environmental variable
 * @name: name of variable
 *
 * Return: 0 if successful
 */
int _unsetenv(const char *name)
{
	char **env_ptr;
	char *buffer;
	int len;

	__attribute__((unused))shvars shvars;

	buffer = _strconcat((char *)name, "=");
	len = _listlen(environ, buffer);
	free(buffer);

	if (len == -1)
	{
		write(STDERR_FILENO, "unsetenv: variable not found\n", 29);
		shvars.status = 2;
		return (SKIP);
	}

	env_ptr = environ + len;
	free(*env_ptr);
	while (*(env_ptr + 1) != NULL)
	{
		*env_ptr = *(env_ptr + 1);
		env_ptr++;
	}
	*env_ptr = NULL;
	shvars.status = 0;

	return (SKIP);
}

/**
 * _cd - changes the current working directory
 * @name: name of directory to change to
 *
 * Return: 0 if successful
 */
int _cd(char *name)
{
	char *home;
	char *pwd;
	char old_path_buffer[PATH_MAX];
	char new_path_buffer[PATH_MAX];
	size_t buf_size = PATH_MAX;
	int i;

	__attribute__((unused))shvars shvars;

	getcwd(old_path_buffer, buf_size);
	if (name == NULL)
	{
		home = _getarrelement(environ, "HOME=");
		if (home == NULL)
		{
			shvars.status = 2;
			_errormsg("cd", name);
			return (SKIP);
		}
		home += 5;
		i = chdir((const char *)home);
		if (i != -1)
			_setenv("PWD", (const char *)home);
	}
	else if (_strcmp("-", name, MATCH) == TRUE)
	{
		pwd = _getarrelement(environ, "OLDPWD=");
		if (pwd == NULL)
		{
			shvars.status = 2;
			_errormsg("cd", name);
			return (SKIP);
		}
		pwd += 7;
		i = chdir((const char *)pwd);
		if (i != -1)
		{
			write(STDOUT_FILENO, pwd, _strlen(pwd));
			write(STDOUT_FILENO, "\n", 1);
			_setenv("PWD", (const char *)pwd);
		}
	}
	else if (name != NULL)
	{
		i = chdir((const char *)name);
		if (i != -1)
			_setenv("PWD", getcwd(new_path_buffer, buf_size));
	}
	if (i == -1)
	{
		shvars.status = 2;
		_errormsg("cd", name);
		return (SKIP);
	}
	shvars.status = 0;
	_setenv("OLDPWD", (const char *)old_path_buffer);
	return (SKIP);
}

/**
 * _aliasfunc - deals with command aliases
 * @args: arguments from command line
 * @to_free: indicated if aliases need to be freed (exiting shell);
 *
 * Return: TRUE if exiting, FALSE if the command is not "alias" or an alias,
 * SKIP if success
 */
int _aliasfunc(char **args, int to_free)
{
	static alias head = {NULL, NULL, NULL};
	char *char_ptr;
	int no_error = TRUE;

	__attribute__((unused))shvars shvars;

	if (to_free == TRUE)
		return (_freealias(head.next));
	if (_strcmp("alias", *args, MATCH) != TRUE)
		return (_checkalias(args, head.next));
	args++;

	if (*args == NULL)
		return (_printalias(head.next));
	while (*args != NULL)
	{
		char_ptr = *args;
		while (*char_ptr != '\0' && *char_ptr != '=')
			char_ptr++;

		if (*char_ptr == '\0' || char_ptr == *args)
		{
			if (_printaliasval(*args, &head) == FALSE)
				no_error = FALSE;
		}
		else
		{
			*char_ptr = '\0';
			char_ptr++;
			_setaliasval(*args, &head, char_ptr);
			*(char_ptr - 1) = '=';
		}
		args++;
	}
	if (no_error == FALSE)
		return (SKIP);
	shvars.status = 0;
	return (SKIP);
}

/**
 * _printenv - prints the environment
 *
 * Return: TRUE
 */
int _printenv(void)
{
	char **ptr = environ;
	
	__attribute__((unused))shvars shvars;

	while (*ptr != NULL)
	{
		write(STDOUT_FILENO, *ptr, _strlen(*ptr));
		write(STDOUT_FILENO, "\n", 1);
		ptr++;
	}

	shvars.status = 0;

	return (SKIP);
}

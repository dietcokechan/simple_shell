#include "shell.h"

/**
 * _getarrelement - gets an element of an arr
 * @arr: arr to be searched
 * @elem_name: name of element to be found
 *
 * Return: the arr element, or NULL if it is not found
 */
char *_getarrelement(char **arr, char *elem_name)
{
	while (*arr != NULL)
	{
		if (_strcmp(*arr, elem_name, PREFIX) == TRUE)
			return (*arr);

		arr++;
	}

	return (NULL);
}

/**
 * _makearray - makes a list from a buffer
 * @str: the buffer
 * @delim: character to mark the end of a list entry
 * @if_sep: if the string has semicolons, if_sep becomes the location after the
 * semicolon
 *
 * Return: a pointer to the list
 */
char **_makearray(char *str, char delim, char **if_sep)
{
	char *str_ptr = str;
	unsigned int i = 2;
	char **arr = NULL;

	while (*str_ptr != '\0')
	{
		if (*str_ptr == ';')
			break;
		if (*str_ptr == delim && *(str_ptr + 1) != '\0')
			i++;
		str_ptr++;
	}
	arr = malloc(i * sizeof(char **));
	if (arr == NULL)
		exit(EXIT_FAILURE);
	arr[0] = str;
	str_ptr = str;
	i = 1;
	while (*str_ptr != '\0')
	{
		if (*str_ptr == delim)
		{
			*str_ptr = '\0';
			str_ptr++;
			if (*str_ptr == ';')
			{
				arr[i] = NULL;
				if (*(str_ptr + 1) != '\0' && *(str_ptr + 2) != '\0')
					*if_sep = str_ptr + 2;
				break;
			}
			if (*str_ptr != '\0')
			{
				arr[i] = str_ptr;
				i++;
			}
		}
		str_ptr++;
	}
	arr[i] = NULL;
	return (arr);
}

/**
 * _listlen - finds the length of a list, or the index of an entry
 * @list: list to be evaluated
 * @entry: entry to be indexed
 *
 * Return: length or index if success, -1 if failure
 */
int _listlen(char **list, char *entry)
{
	int i = 0;

	if (entry == NULL)
	{
		while (*list != NULL)
		{
			i++;
			list++;
		}
		i++;
		return (i);
	}
	else
	{
		while (*list != NULL)
		{
			if (_strcmp(*list, entry, PREFIX) == TRUE)
				return (i);

			i++;
			list++;
		}
	}

	return (-1);
}

/**
 * _arrcpy - copies an arr
 * @old_arr: arr to be copied
 * @new_size: size of new arr
 *
 * Return: the new arr
 */
char **_arrcpy(char **old_arr, int new_size)
{
	char **new_arr = NULL;
	char **ptr_arr;

	new_arr = malloc(sizeof(char **) * new_size);

	ptr_arr = new_arr;
	while (*old_arr != NULL)
	{
		*ptr_arr = _strdup(*old_arr);
		ptr_arr++;
		old_arr++;
	}
	*ptr_arr = NULL;

	return (new_arr);
}

/**
 * _freearr - frees a two dimensional arr
 * @args: arr to be freed
 *
 * Return: TRUE
 */
int _freearr(char **args)
{
	char **ptr = args;

	while (*ptr != NULL)
	{
		free(*ptr);
		ptr++;
	}

	free(args);

	return (TRUE);
}

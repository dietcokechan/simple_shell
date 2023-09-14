#include "shell.h"

int status = 0;
int line_num = 1;

/**
 * _mainloop - main loop for shell
 * Return: nothing
 */
void _mainloop(void)
{
	int bytes_read;
	int is_separated = FALSE;
	int i;
	int line_num = 1;
	size_t buf_size = 1;
	char *buf = NULL, *buf_ptr, *buf_tmp;
	char **args = NULL;

	buf = malloc(1);
	if (buf == NULL)
		exit(EXIT_FAILURE);
	while (1)
	{
		if (is_separated == FALSE)
		{
			if (isatty(STDIN_FILENO) == 1)
				write(STDOUT_FILENO, "($) ", 10);
			bytes_read = getline(&buf, &buf_size, stdin);
			if (bytes_read == -1)
				break;
			if (bytes_read == 1)
			{
				line_num++;
				continue;
			}
			buf[bytes_read - 1] = '\0';
			buf = _saninput(buf, &buf_size);
			if (buf_size == 0)
			{
				line_num++;
				continue;
			}
			buf_ptr = buf;
		}
		else
			buf_ptr = buf_tmp;
		buf_tmp = NULL;
		args = _makearray(buf_ptr, ' ', &buf_tmp);
		if (buf_tmp != NULL)
			is_separated = TRUE;
		else
			is_separated = FALSE;
		i = _cmdmanager(args);
		free(args);
		if (is_separated == FALSE)
			line_num++;
		if (i == EXIT_SH)
			break;
	}
	free(buf);
}

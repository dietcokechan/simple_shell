#include "shell.h"

/**
 * main - executes commands from the terminal
 * @argc: number of inputs from main
 * @argv: arr of inputs from main
 *
 * Return: 0, or another number if desired
 */
int main(__attribute__((unused))int argc, char **argv)
{
	_shellname = NULL;
	_shellname = _strdup(*argv);

	environ = _arrcpy(environ, _listlen(environ, NULL));

	signal(SIGINT, SIG_IGN);

	_mainloop();
	_aliasfunc(NULL, TRUE);
	_freearr(environ);
	free(_shellname);

	return (status % 256);
}

#include "shell.h"

int status;
char *_shellname = NULL;

/**
 * _myexit - exit shell builtin
 * @status: exit status
 * Return: status
 */
int _myexit(int status)
{
	exit(status);
}

/**
 * pipedsys - piped system
 * @args: arguments
 * @cmd_name: command
 * Return: nothing
 */
void pipedsys(char **args, char *cmd_name)
{
	pid_t pid;

	pid = fork();
	if (pid == 0)
	{
		execve(cmd_name, args, environ);
		perror("Error: ");
	}
	else if (pid < 0)
		perror("Error: ");
}

/**
 * main - executes commands from the terminal
 * @argc: number of inputs from main
 * @argv: arr of inputs from main
 *
 * Return: 0, or another number if desired
 */
int main(__attribute__((unused))int argc, char **argv)
{
	char *_shellname = NULL;

	_shellname = _strdup(*argv);
	environ = _arrcpy(environ, _listlen(environ, NULL));

	signal(SIGINT, SIG_IGN);
	_mainloop();
	_aliasfunc(NULL, TRUE);

	_freearr(environ);
	free(_shellname);

	return (status);
}

#include "shell.h"

/**
 * execmd - executes command
 * @argv: arg vector
 * Return: nothing
 */
void execmd(char **argv)
{
	char *cmd = NULL, *acmd = NULL;

	if (argv)
	{
		cmd = argv[0];
        acmd = getloc(cmd);
		if (execve(acmd, argv, NULL) == -1)
		{
			perror("Error");
		}
	}
}

#include "shell.h"

/**
 * getloc - get our location
 * @cmd: command
 * Return: location
 */
char *getloc(char *cmd)
{
	char *path, *pathcpy, *pathtok, *filepath;
	int cmdlen, dirlen;
	struct stat buffer;

	path = getenv("PATH");
	if (path)
	{
		pathcpy = strdup(path);
		cmdlen = strlen(cmd);
		pathtok = strtok(pathcpy, ":");

		while (pathtok != NULL)
		{
			dirlen = strlen(pathtok);
			filepath = malloc(cmdlen + dirlen + 2);
			strcpy(filepath, pathtok);
			strcat(filepath, "/");
			strcat(filepath, cmd);
			strcat(filepath, "\0");

			if (stat(filepath, &buffer) == 0)
			{
				free(pathcpy);
				return (filepath);
			}
			else
			{
				free(filepath);
				pathtok = strtok(NULL, ":");
			}
		}
        free(pathcpy);
        if (stat(cmd, &buffer) == 0)
            return (cmd);
        return (NULL);
	}
	return (NULL);
}

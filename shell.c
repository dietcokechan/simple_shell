#include "shell.h"

/**
 * main - entry point
 * @argc: arg count
 * @argv: arg vector
 * Return: always 0
 */
int main(int argc, char **argv)
{
	char *prompt = "($) ", *token;
	char *line, *linecpy = NULL;
	size_t n = 0;
	ssize_t nchars;
	const char *delim = " \n";
	int ntokens = 0, i;

	(void)argc;
	while (1)
	{
		printf("%s", prompt);
		nchars = getline(&line, &n, stdin);
		if (nchars == -1)
		{
			printf("Exiting...\n");
			return (-1);
		}
		linecpy = malloc(sizeof(char) * nchars);
		if (linecpy == NULL)
		{
			perror("Memory allocation error");
			return (-1);
		}
		strcpy(linecpy, line);
		token = strtok(line, delim);
		while (token != NULL)
		{
			ntokens++;
			token = strtok(NULL, delim);
		}
		ntokens++;
		argv = malloc(sizeof(char *) * ntokens);
		token = strtok(linecpy, delim);
		for (i = 0; token != NULL; i++)
		{
			argv[i] = malloc(sizeof(char) * strlen(token));
			strcpy(argv[i], token);
			token = strtok(NULL, delim);
		}
		argv[i] = NULL;
		execmd(argv);
	}
	free(linecpy);
	free(line);
	return (0);
}

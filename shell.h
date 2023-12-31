#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <limits.h>
#include <signal.h>

#define FALSE 0
#define TRUE 1
#define NEITHER 2
#define MATCH 3
#define PREFIX 4
#define EXIT_SH 5
#define SKIP 6
#define EXECVE 7

/**
 * struct Alias - singly linked list
 * @name: name of alias
 * @value: command that alias calls
 * @next: points to next node
 */
typedef struct Alias
{
	char *name;
	char *value;
	struct Alias *next;
} alias;

extern char **environ;
char *_shellname;
int status;
int line_num;

void _setshellname(char **argv);
char *_getshellname(void);
void _mainloop(void);

int _cmdmanager(char **args);
int _builtins(char **args);
int and_or(char **args, char operator, int last_compare);
char *_checkcmd(char **args);
int _execmd(char **args);

char *_saninput(char *old_buf, size_t *old_size);
int _checkinput(char *ptr);
void _errormsg(char *arg0, char *arg1);
char *_checkvars(char *arg);

void _assignlineptr(char **lineptr, size_t *n, char *buffer, size_t b);
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size);
int _getline(char **line_ptr, size_t *n, FILE *file);
int _myexit(int status);

int _strlen(char *str);
char *_strdup(char *src);
char *_strconcat(char *s1, char *s2);
int _strcmp(char *s1, char *s2, int pref_or_match);

char *_getarrelement(char **arr, char *elem_name);
char **_makearray(char *str, char delim, char **if_sep);
int _listlen(char **list, char *entry);
char **_arrcpy(char **old_arr, int new_size);
int _freearr(char **args);

int _setenv(const char *name, const char *value);
int _unsetenv(const char *name);
int _printenv(void);
int _cd(char *name);
int _aliasfunc(char **args, int free);

int _freealias(alias *alias_ptr);
int _checkalias(char **args, alias *alias_ptr);
int _printalias(alias *alias_ptr);
int _printaliasval(char *arg, alias *alias_ptr);
int _setaliasval(char *arg, alias *alias_ptr, char *new_value);

char *_itoa(int n);
int _atoi(char *s);

#endif

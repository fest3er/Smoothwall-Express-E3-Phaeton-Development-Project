/* SmoothWall helper program - header file
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * filename: setuid.h
 * Simple header file for all setuid progs. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdarg.h>   
#include <fcntl.h> 
#include <syslog.h>
#include <signal.h>
#include <grp.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define STRING_SIZE 256
#define LETTERS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define NUMBERS "0123456789"
#define NUMBERS_COLON "0123456789:"
#define LETTERS_NUMBERS LETTERS NUMBERS
#define IP_NUMBERS "./" NUMBERS

#define MAX_ARGS 100

#define MSG_RUNNING_COMMAND "Running command: "
#define MSG_ARG_START "["
#define MSG_ARG_END "] "
#define MSG_NEWLINE "\n"

int initsetuid(int closefd);
void cleanupsetuid(void);
void stripnl(char *s);
int simplesecuresysteml(char *arg, ...);
int simplesecuresystemv(char *argv[]);
int simplesecurepopenl(pid_t *ppid, char *arg, ...);
int simplesecurepopenv(pid_t *ppid, char *argv[]);

int nullfd = -1;

int initsetuid(int closefd)
{
	if (closefd)
	{
		if ((nullfd = open("/dev/null", O_WRONLY, 0)) == -1) {
			fprintf(stderr, "Couldn't open /dev/null\n"); return 0; }
	}
	if (setgroups(0, NULL)) { fprintf(stderr, "Couldn't clear group list\n"); exit(1); }
	if (setgid(0)) { fprintf(stderr, "Couldn't set GID to 0\n"); return 0; }
	if (setuid(0)) { fprintf(stderr, "Couldn't set UID to 0\n"); return 0; }

	return 1;
}

void cleanupsetuid(void)
{
	if (nullfd != -1)
		close(nullfd);
}

/* stripnl().  Replaces \n with \0 */
void stripnl(char *s)
{
	char *t = strchr(s, '\n');
	if (t) *t = '\0';
}

/* This is an alternative to mysystem(), that uses a fork(),exec() combination
 * to avoid the perils of calling the shell. */
int simplesecuresysteml(char *arg, ...)
{
	va_list ap;
	char *p = NULL;
	char *argv[MAX_ARGS];
	int ret = 0, n = 0;
	
	memset(argv, 0, sizeof(char *) * MAX_ARGS);
	
	argv[n++] = (char *) strdup(arg);	
	va_start(ap, arg);
	while ((p = va_arg(ap, char *)) && (n < MAX_ARGS - 1)) {
		argv[n++] = (char *) strdup(p); }
	argv[n] = NULL;
	va_end(ap);
	
	ret = simplesecuresystemv(argv);
	
	for (n = 0; argv[n]; n++) free(argv[n]);
	
	return ret;
}

int simplesecuresystemv(char *argv[])
{	
	pid_t pid = 0;
	int status = 0;
	int n = 0;
	
	pid = fork();
	if (pid == -1)
		return -1;
	else if (pid == 0)
	{
		dup2(nullfd, 0);
		dup2(nullfd, 1);
		close(nullfd);

		write(2, MSG_RUNNING_COMMAND, strlen(MSG_RUNNING_COMMAND));
		for (n = 0; argv[n]; n++)
		{
			write(2, MSG_ARG_START, strlen(MSG_ARG_START));
			write(2, argv[n], strlen(argv[n]));
			write(2, MSG_ARG_END, strlen(MSG_ARG_END));
		}
		write(2, MSG_NEWLINE, strlen(MSG_NEWLINE));
				
		exit (execve(argv[0], argv, NULL));
		// otherwise we are running the command and will not return.		
	}
	else
	{
		waitpid(pid, &status, 0);
		return (WEXITSTATUS(status));
	}	

	return 0;	
}


/* Secure alternative to popen(). */
int simplesecurepopenl(pid_t *ppid, char *arg, ...)
{
	va_list ap;
	char *p;
	char *argv[MAX_ARGS];
	int n = 0;
	int ret = 0;
	
	argv[n++] = (char *) strdup(arg);
	va_start(ap, arg);
	while ((p = va_arg(ap, char *)) && (n < MAX_ARGS - 1)) {
		argv[n++] = (char *) strdup(p); }
	argv[n] = NULL;
	va_end(ap);
	
	ret = simplesecurepopenv(ppid, argv);
	
	for (n = 0; argv[n]; n++) free(argv[n]);

	return ret;
}


int simplesecurepopenv(pid_t *ppid, char *argv[])
{
	int pipefds[2] = { 0, 0 };
	int n;
	
	if (pipe(pipefds) == -1) { return -1; }

	*ppid = fork();
	if (*ppid == -1)
	{
		/* error */
		close(pipefds[0]);
		close(pipefds[1]);
		return -1;
	}
	else if (*ppid == 0)
	{
		/* in child */
		dup2(nullfd, 0);
		dup2(pipefds[1], 1);	
		close(pipefds[0]);
		close(pipefds[1]);

		write(2, MSG_RUNNING_COMMAND, strlen(MSG_RUNNING_COMMAND));
		for (n = 0; argv[n]; n++)
		{
			write(2, MSG_ARG_START, strlen(MSG_ARG_START));
			write(2, argv[n], strlen(argv[n]));
			write(2, MSG_ARG_END, strlen(MSG_ARG_END));
		}
		write(2, MSG_NEWLINE, strlen(MSG_NEWLINE));
		
		exit (execve(argv[0], argv, NULL));
		// otherwise we are running the command and will not return.		
	}

	close(pipefds[1]);
	
	return pipefds[0];
}

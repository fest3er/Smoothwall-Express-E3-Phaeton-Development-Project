/* SmoothWall helper program - header file
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * filename: setuid.h
 * Simple header file for all setuid progs. */

#include "setuid.h"

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
#include <sys/un.h>
//#include <newt.h>
#include <dirent.h>
#include <stdarg.h>   
#include <fcntl.h> 
#include <syslog.h>
#include <signal.h>
#include <grp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#include <vector>
#include <string>
#include <cstring>

int nullfd = -1;

#define INPUT_BUFFER 128
#define TIMEOUT 30

extern int errno;

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

int simplesecuresysteml(const char *arg, ...)
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

int simplesecuresystemv(char *const argv[])
{
	return simplesecuresystemvenv(argv,NULL);
}

int simplesecuresystemvenv(char *const argv[], char *const envp[])
{	
	pid_t pid = 0;
	int status = 0;
	int n = 0;
	
	pid = fork();
	if (pid == -1)
		return -1;
	else if (pid == 0)
	{
		int pgrp = getpid();
		setpgid(0,pgrp);
		
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
				
		_exit (execve(argv[0], argv, envp));
		// otherwise we are running the command and will not return.		
	}
	else
	{
		waitpid(pid, &status, 0);
		return (WEXITSTATUS(status));
	}	

	return 0;	
}

int simplesecuresystemvector(std::vector<std::string> &args)
{
	char *arglist[args.size() + 1];
	unsigned int c = 0;
	
	for (c = 0; c < args.size(); c++)
		arglist[c] = (char *) args[c].c_str();
	arglist[c] = NULL;
	
	return (simplesecuresystemv(arglist));
}

/* Secure alternative to popen(). */
int simplesecurepopenl(int writing, pid_t *ppid, char *const arg, ...)
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
	
	ret = simplesecurepopenv(writing, ppid, argv);
	
	for (n = 0; argv[n]; n++) free(argv[n]);

	return ret;
}

int simplesecurepopenv(int writing, pid_t *ppid, char *const argv[])
{
	return simplesecurepopenvenv(writing, ppid, argv, NULL);
}

int simplesecurepeopenvector(int writing, pid_t *ppid, std::vector<std::string> &args)
{
	char *arglist[args.size() + 1];
	unsigned int c = 0;
	
	for (c = 0; c < args.size(); c++)
		arglist[c] = (char *) args[c].c_str();
	arglist[c] = NULL;
	
	return (simplesecurepopenv(writing, ppid, arglist));
}

int simplesecurepopenvenv(int writing, pid_t *ppid, char *const argv[], char *const envp[])
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
		if (!writing)
		{
			dup2(nullfd, 0);
			dup2(pipefds[1], 1);
		}
		else
		{
			dup2(pipefds[0], 0);
			dup2(nullfd, 1);
		}		
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
		
		exit (execve(argv[0], argv, envp));
		// otherwise we are running the command and will not return.		
	}
	
	if (!writing)
	{
		close(pipefds[1]);
		return pipefds[0];
	}
	else
	{
		close(pipefds[0]);
		return pipefds[1];
	}
}

int simplesecurepopenextl( int * pipefds, pid_t *ppid, const char *arg, ...)
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
	
	ret = simplesecurepopenextv(pipefds, ppid, argv);
	
	for (n = 0; argv[n]; n++) free(argv[n]);

	return ret;
}

int simplesecurepopenextv( int * pipefds, pid_t *ppid, char *const argv[] )
{
	pipefds[ 0 ] = 0;
	pipefds[ 1 ] = 0;
	int n;
	pid_t pid = 0;
	int status = 0;

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
	else
	{
		waitpid(pid, &status, 0);
		return (WEXITSTATUS(status));
	}	
	return 0;
}

// this returns a vector of lines that the external command 
// produces on stdout when run as args with the vector of input fed as
// a number of lines.
// the input lines should normally be newline terminated if the called program
// needs to treat them as individual records.
std::vector<std::string> simplesecurepopenvector(std::vector<std::string> &args,
                                                 std::vector<std::string> &input) {
    char *arglist[args.size() + 1];
    char *inputlist[input.size() + 1];
    std::vector<std::string> output; // what the command returns on stdout
    pid_t ppid = 0;
    pid_t mypid = 0;
    pid_t reaped_pid;
    int stdinpipefds[2],stdoutpipefds[2];
    unsigned int c = 0;
    int status = 0;

    FILE *infd;
    char buf[1024];
    // get pointers to the indivdual items in the vectors
    // for the arguments
    for (c = 0; c < args.size(); c++)
        arglist[c] = (char *) args[c].c_str();
    arglist[c] = NULL;
    // and what we will be feeding it
    for (c = 0; c < input.size(); c++)
        inputlist[c] = (char *) input[c].c_str();
    inputlist[c] = NULL;

    stdinpipefds[0] = 0;
    stdinpipefds[1] = 0;
    stdoutpipefds[0] = 0;
    stdoutpipefds[1] = 0;

    if(pipe(stdinpipefds) != -1 && pipe(stdoutpipefds) != -1) {
        ppid = fork();
        if(ppid == -1) {
		    // primary fork failure
            close(stdinpipefds[0]);
            close(stdinpipefds[1]);
            close(stdoutpipefds[0]); 
            close(stdoutpipefds[1]);
            return output; // empty vec
        }
        else if(ppid == 0) {
            // in first child
            dup2(stdinpipefds[0], 0);
            dup2(stdoutpipefds[1], 1);
            close(stdinpipefds[0]);
            close(stdinpipefds[1]);
            close(stdoutpipefds[0]); 
            close(stdoutpipefds[1]);
            write(2, MSG_RUNNING_COMMAND, strlen(MSG_RUNNING_COMMAND));
            for (c = 0; arglist[c]; c++)
			{
            	write(2, MSG_ARG_START, strlen(MSG_ARG_START));
                write(2, arglist[c], strlen(arglist[c]));
                write(2, MSG_ARG_END, strlen(MSG_ARG_END));
			}	
            write(2, MSG_NEWLINE, strlen(MSG_NEWLINE));
		
            exit (execve(arglist[0], arglist, NULL));
        }
        else {
	    // in parent
            // ppid contains pid of spawned process...
            
            // spawn a second child to send our data
	    mypid = fork();
            if(mypid == -1) {
		// secondary fork failure
	        kill(ppid, SIGHUP); // so kill the 1st child
            	close(stdinpipefds[0]);
            	close(stdinpipefds[1]);
            	close(stdoutpipefds[0]); 
            	close(stdoutpipefds[1]);
            	return output; // empty vec
	    }
            else if(mypid == 0) {
				
                close(stdoutpipefds[0]); 
                close(stdoutpipefds[1]); 
                close(stdinpipefds[0]); 
                // we are the child so write to pipefds[1]
                for(c = 0; inputlist[c]; c++) {
                    write(stdinpipefds[1], inputlist[c], strlen(inputlist[c]));
                }
                close(stdinpipefds[1]);
                exit(0); // sender child is done
            }
            else {
                // parent
                close(stdinpipefds[0]); 
                close(stdinpipefds[1]); 
                close(stdoutpipefds[1]); 
            
                if((infd = fdopen(stdoutpipefds[0],"r")) != NULL) {
                    while(fgets(buf, sizeof(buf), infd) != NULL)
                        output.push_back(buf);
                }
                fclose(infd);
                close(stdoutpipefds[0]);
		// and reap two outstanding processes
		do {
			reaped_pid = waitpid(-1, &status, 0);
			// cross of the processes we are wating for - status not interesting
			if(reaped_pid == ppid) {
			    ppid = 0;
			}
			else if(reaped_pid == mypid) {
			    mypid = 0;
			}
			else {
			  // seem to have a bastard
			  // really should log this somewhere
			  fprintf(stderr, "Reaped unknown pid %d not %d or %d\n", reaped_pid,ppid,mypid);
			  ppid = 0;
			  mypid = 0;
			}
		} while(ppid || mypid);
		// this loop ends when both children are accounted for
			    
            }

        }
    }
    return output; // may be empty if things went wrong
}

std::string calculatemd5(const char *filename)
{        
	std::vector<std::string> md5cmd;
	std::vector<std::string> md5input;
	std::vector<std::string> md5result;
	
	md5cmd.push_back("/usr/bin/md5sum");
	md5cmd.push_back(filename);
	
	md5result = simplesecurepopenvector(md5cmd, md5input);
	
	return md5result[0].substr(0, 32);
}

std::string calculatemd5(std::string filename)
{
	return calculatemd5(filename.c_str());
}

std::string message( std::vector<std::string> parameters )
{
	/* go through the list of parameters and add them into a nice long space seperated */
	/* string which we can then terminate and pass on to smoothd */

	std::string sendableMessageString = "";

	for ( std::vector<std::string>::iterator i = parameters.begin() ; i != parameters.end() ; i++ ){
		sendableMessageString += (*i);
		sendableMessageString += " ";
	}

	return message( sendableMessageString );
}

std::string message( std::string messageString )
{
	std::string sendableMessageString = messageString;
	sendableMessageString += '\n';
	int sockfd;
	int len;
	struct sockaddr_un address;
	int result;
	char inputBuffer[INPUT_BUFFER];
	timeval timeOut;
	
	sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

	if(sockfd < 0) {
		syslog(LOG_NOTICE, "Unable to bind socket for communications with SmoothD for %s",  messageString.c_str());
		return( "" );
	}
	
	address.sun_family = AF_UNIX;
	strcpy(address.sun_path, "/dev/smoothd");
	len = sizeof(address);

	timeOut.tv_sec = TIMEOUT;
	timeOut.tv_usec = 0;

	if(setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeOut, sizeof(timeOut))) {
		syslog(LOG_NOTICE, "Couldn't set timeout on socket for communications with SmoothD for %s", messageString.c_str());
		return( "" );
	}
	result = connect(sockfd, (struct sockaddr *)&address, len);

	if(result == -1) {
		syslog(LOG_NOTICE, "Unable to connect socket for communications with SmoothD for %s",  messageString.c_str());
		return( "" );
	}

	// write parameters
	write(sockfd, sendableMessageString.c_str(), sendableMessageString.length());

	int inputCharCount = read(sockfd, inputBuffer, sizeof(inputBuffer));

	if((inputCharCount == 0) || (errno == EAGAIN) || (errno == EWOULDBLOCK)) {
		syslog(LOG_NOTICE, "No response from SmoothD for %s",  messageString.c_str());
		return( "" );
	}
	
	if(inputCharCount < INPUT_BUFFER) {
		inputBuffer[inputCharCount] = '\0';
	} else {
		inputBuffer[INPUT_BUFFER-1] = '\0';
	}

	if(!strcmp(inputBuffer,"Attempt to invoke unregistered command\n")) {
		return( "" );
	}
	
	close(sockfd);
	return( inputBuffer );
}

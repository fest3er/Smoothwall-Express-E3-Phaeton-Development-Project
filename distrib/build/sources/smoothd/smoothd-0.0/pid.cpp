#include <fstream>
#include <cstdlib>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <syslog.h>
#include "module.h"

#define NUMBERS_CR "0123456789\n"

void create_pid_file( const char * pidfilename )
{
	/* retrieve our PID */

	pid_t our_pid = getpid( );

	/* write our PID out to our PID file */
	std::ofstream pid_file( pidfilename );

	pid_file << our_pid << "\n";

	pid_file.close();
	return;
}

void remove_pid_file( const char * pidfilename )
{
	/* remove the pid file */
	unlink( pidfilename );
	return;
}

int amrunning( const char * pidfilename, const char * processname )
{
	/* check to see whether this application is already running or not */
	
	/* first of all open the pid file and extract our process ID */
	
	char buffer[ 255 ];
	
	int fd = -1;
	int pid = 0;
	
	memset(buffer, 0, 255);
	
	if ((fd = open( pidfilename, O_RDONLY)) == -1)
	{
		return 0;
	}
	if (read( fd, buffer, 255 - 1) == -1)
	{
		return 0;
	}

	buffer[ strlen( buffer ) - 1 ] = '\0';	
	if (strspn( buffer, NUMBERS_CR) != strlen( buffer ))
	{
		return 0;
	}
	
	pid = safeatoi(buffer);
	
	if (pid <= 1)
	{
		return 0;
	}

	/* we have a pid, and a pid file */
	
	snprintf( buffer, sizeof( buffer ), "/proc/%d/status", pid );
	
	if ((fd = open( buffer, O_RDONLY)) == -1)
	{
		return 0;
	}

	/* the buffer should now *hopefully* contain the line name: XXXX */
	/* where xxx is our process name :)                              */

	bool found = false;
	
	while (read( fd, buffer, 255 - 1) > 0)
	{
		if ( strstr( buffer, "Name:" ) ){
			/* we've found the name line */
			if ( strstr( buffer, processname ) ){
				found = true;
			}
			break;
		}
	}
	
	if ( found ) return pid;

	return 0;
}

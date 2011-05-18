/* SmoothWall helper program - restartdhcp
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * (c) Lawrence Manning, 2001
 *
 * filename: restartdhcp
 * Simple program intended to be installed setuid(0) that can be used for
 * restarting DHCPd. */

#include "setuid.h"

int main(void)
{
	int fd = -1;
	char buffer[STRING_SIZE];
	char green[STRING_SIZE];
	char purple[STRING_SIZE];
	char command[STRING_SIZE];
	int pid;
	
	if (!(initsetuid(1)))
		exit(1);
		
	memset(buffer, 0, STRING_SIZE);
	memset(green, 0, STRING_SIZE);
	memset(purple, 0, STRING_SIZE);
	memset(command, 0, STRING_SIZE);
	
	if ((fd = open("/var/smoothwall/dhcp/green",  O_RDONLY)) == -1)
	{
		goto RESTART;
	}
	if (read(fd, green, STRING_SIZE - 1) == -1)
	{
		fprintf(stderr, "Couldn't read green file");
		goto EXIT;
	}
        close(fd);
        fd = -1;	

        if ((fd = open("/var/smoothwall/dhcp/purple",  O_RDONLY)) == -1)
        {
        	goto RESTART;
	}
	if (read(fd, purple, STRING_SIZE - 1) == -1)
	{
		fprintf(stderr, "Couldn't read purple file");
		goto EXIT;
	}
	close(fd);
	fd = -1;
	
	if ((fd = open("/var/run/dhcpd.pid", O_RDONLY)) == -1)
	{
		goto RESTART;
	}
	if (read(fd, buffer, STRING_SIZE - 1) == -1)
	{
		fprintf(stderr, "Couldn't read from pid file");
		goto EXIT;
	}
	close(fd);
	fd = -1;
	
	pid = atoi(buffer);
	if (pid <= 1)
	{
		fprintf(stderr, "Bad pid value");
		goto EXIT;
	}
	if (kill(pid, SIGTERM) == -1)
	{
		fprintf(stderr, "Unable to send SIGTERM");
		goto EXIT;
	}
	unlink("/var/run/dhcpd.pid");
	
EXIT:
	if (fd != -1)
		close(fd);
	
RESTART:
	if ((fd = open("/var/smoothwall/dhcp/enable", O_RDONLY)) != -1)
	{
		snprintf(command, STRING_SIZE - 1, "/usr/sbin/dhcpd %s %s", green, purple);
		system(command);
		close(fd);
	}

	return 0;
}

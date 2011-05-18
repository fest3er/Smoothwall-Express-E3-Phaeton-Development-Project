/* SmoothWall helper program - restartssh
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * (c) Lawrence Manning, 2001
 *
 * filename: restartssh
 * Simple program intended to be installed setuid(0) that can be used for
 * restarting sshd. */

#include "setuid.h"

int main(void)
{
	int fd = -1;
	char buffer[STRING_SIZE];
	int pid = 0;
	
	if (!(initsetuid(1)))
		goto EXIT;
		
	if ((fd = open("/var/run/sshd.pid", O_RDONLY)) == -1)
	{
		goto RESTART;
	}
	if (read(fd, buffer, STRING_SIZE - 1) == -1)
	{
		fprintf(stderr, "Couldn't read from pid file");
		goto EXIT;
	}
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
	unlink("/var/run/sshd.pid");
	
EXIT:
	if (fd != -1)
		close(fd);
	
RESTART:
	if ((fd = open("/var/smoothwall/remote/enablessh", O_RDONLY)) != -1)
	{
		system("/usr/sbin/sshd");
		close(fd);
	}

	return 0;
}

/* SmoothWall helper program - setadvnet
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * (c) Lawrence Manning, 2001
 *
 * filename: setadvnet.c
 * Restarting advanced networking features. */
 
#include "setuid.h"

int main(void)
{
	int fd = -1;
	char noping[STRING_SIZE];
	char cookies[STRING_SIZE];

	memset(noping, 0, STRING_SIZE);
	memset(cookies, 0, STRING_SIZE);
	
	if (!(initsetuid(1)))
		goto EXIT;
	
        if ((fd = open("/var/smoothwall/advnet/noping", O_RDONLY)) != -1)
        {
        	close(fd);
		strncpy(noping, "1\n", STRING_SIZE - 1);
	}
	else
		strcpy(noping, "0\n");

        if ((fd = open("/var/smoothwall/advnet/cookies", O_RDONLY)) != -1)
        {
        	close(fd);
		strncpy(cookies, "1\n", STRING_SIZE - 1);
	}
	else
		strncpy(cookies, "0\n", STRING_SIZE - 1);
	
	if ((fd = open("/proc/sys/net/ipv4/icmp_echo_ignore_all", O_WRONLY)) != -1)
	{
		write(fd, noping, 2);
		close(fd);
	}

	if ((fd = open("/proc/sys/net/ipv4/tcp_syncookies", O_WRONLY)) != -1)
	{
		write(fd, cookies, 2);
		close(fd);
	}

	system("/sbin/iptables -F advnet");
	
	if ((fd = open("/var/smoothwall/advnet/noigmp", O_RDONLY)) != -1)
	{   
		close(fd);
		system("/sbin/iptables -A advnet -p igmp -j DROP");
	}
	
	if ((fd = open("/var/smoothwall/advnet/nomulticast", O_RDONLY)) != -1)
	{   
		close(fd);		
		system("/sbin/iptables -A advnet -d 224.0.0.0/4 -j DROP");
	}

EXIT:

	return 0;
}

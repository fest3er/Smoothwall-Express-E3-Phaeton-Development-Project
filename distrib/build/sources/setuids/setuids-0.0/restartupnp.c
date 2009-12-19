/* SmoothWall helper program - restartupnp
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * (c) Lawrence Manning, 2001
 *
 * filename: restartupnp.c
 * Restarting UPnP support. */
 
#include "setuid.h"

int main(void)
{
	int fd = -1;
	FILE *ifacefile;
	char iface[STRING_SIZE];
	char command[STRING_SIZE];
	
	if (!(initsetuid(0)))
		exit(1);
	
	system("/usr/bin/killall upnpd >/dev/null 2>/dev/null");
	system("/sbin/route del -net 239.0.0.0 netmask 255.0.0.0");

	if (!(ifacefile = fopen("/var/smoothwall/red/iface", "r")))
	{
		fprintf(stderr, "Couldn't open iface file");
		exit(0);
	}
	fgets(iface, STRING_SIZE, ifacefile);
	if (iface[strlen(iface) - 1] == '\n')
	iface[strlen(iface) - 1] = '\0';        
	fclose(ifacefile);
	ifacefile = NULL;
	if (strspn(iface, LETTERS_NUMBERS) != strlen(iface))
	{
		fprintf(stderr, "Bad iface: %s\n", iface);
		exit(0);
	}

        if ((fd = open("/var/smoothwall/advnet/upnp", O_RDONLY)) != -1)
        {
        	close(fd);
        	system("/sbin/route add -net 239.0.0.0 netmask 255.0.0.0 eth0");
        	snprintf(command, STRING_SIZE -1, "/usr/sbin/upnpd %s eth0 >/dev/null 2>/dev/null", iface);
               	system(command);
	}

	return 0;
}

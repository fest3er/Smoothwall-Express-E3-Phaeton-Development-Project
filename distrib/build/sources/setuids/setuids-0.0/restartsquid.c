/* SmoothWall helper program - restartsquid
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * (c) Lawrence Manning, 2001
 *
 * filename: restartsquid.c
 * Restarting squid with transparent proxying. */
 
#include "setuid.h"

int main(void)
{
	int fd = -1;
	int enable = 0;
	int transparent = 0;
	int running = 0;
	int clearedcache = 0;
	
	if (!(initsetuid(1)))
		exit(1);

	system("/sbin/iptables -t nat -F squid");
	
	if ((fd = open("/var/smoothwall/proxy/enable", O_RDONLY)) != -1)
	{
		close(fd);
		enable = 1;
	}
	if ((fd = open("/var/smoothwall/proxy/transparent", O_RDONLY)) != -1)
	{
		close(fd);
		transparent = 1;
	}
	if ((fd = open("/var/run/squid.pid", O_RDONLY)) != -1)
	{
		close(fd);
		running = 1;
	}
	if ((fd = open("/var/smoothwall/proxy/clearedcache", O_RDONLY)) != -1)
	{
		close(fd);
		clearedcache = 1;
	}
	
	system("/usr/bin/killall squid >/dev/null 2>/dev/null");
	sleep(5);
	
	if (clearedcache)
	{
		system("/bin/rm -rf /var/squid/cache" );
		system("/bin/mkdir /var/squid/cache");
		system("/bin/chown squid:squid /var/spool/squid/cache");

	}
	
	if (enable)
	{
		system("/usr/sbin/squid -D -z -f /var/smoothwall/proxy/squid.conf"); 
		system("/usr/sbin/squid -D -f /var/smoothwall/proxy/squid.conf");
	}
	
	if (transparent && enable)
		system("/sbin/iptables -t nat -A squid -p tcp --dport 80 -j REDIRECT --to-ports 800");

	system("/bin/chown squid:squid /var/spool/squid/cache");

	unlink("/var/smoothwall/proxy/clearedcache");	
					
	return 0;
}

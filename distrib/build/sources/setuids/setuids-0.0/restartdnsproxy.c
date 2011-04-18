/* SmoothWall helper program - restartsnort
 *
 * (c) The SmoothWall Team, 2002
 *
 * filename: restartdnsproxy
 * Restarting dnsproxy. */
 
#include "setuid.h"

int main(int argc, char *argv[])
{
	FILE *resolvfile = NULL;
	char dns1[STRING_SIZE];
	char dns2[STRING_SIZE];
	
	memset(dns1, 0, STRING_SIZE);
	memset(dns2, 0, STRING_SIZE);

	if (!(initsetuid(1)))
		exit(1);
	
	if (argc >= 2)
		strncpy(dns1, argv[1], STRING_SIZE - 1);
	if (argc >= 3)
		strncpy(dns2, argv[2], STRING_SIZE - 1);
		
	if (strspn(dns1, IP_NUMBERS) != strlen(dns1))
	{
		fprintf(stderr, "Bad DNS1 IP %s\n", dns1);
		goto EXIT;
	}
	if (strspn(dns2, IP_NUMBERS) != strlen(dns2))
	{
		fprintf(stderr, "Bad DNS2 IP %s\n", dns2);
		goto EXIT;
	}
	if (!(resolvfile = fopen("/etc/resolv.conf.dnsmasq", "w")))
	{
		fprintf(stderr, "Couldn't write to /etc/resolv.conf.dnsmasq\n");
			goto EXIT;
	}
	if (strlen(dns1))
		fprintf(resolvfile, "nameserver %s\n", dns1);
	if (strlen(dns2))
		fprintf(resolvfile, "nameserver %s\n", dns2);
	fclose(resolvfile);
				
	system("/usr/bin/killall dnsmasq");

	sleep(1);
	
	system("/usr/bin/dnsmasq -r /etc/resolv.conf.dnsmasq");
	
EXIT:

	return 0;
}

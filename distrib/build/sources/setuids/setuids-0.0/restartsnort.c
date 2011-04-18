/* SmoothWall helper program - restartsnort
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * (c) Lawrence Manning, 2001
 *
 * filename: restartsnort.c
 * Restarting snort. */
 
#include "setuid.h"

int main(void)
{
	int fd = -1;
	FILE *ifacefile, *ipfile, *dns1file, *dns2file, *varsfile;
	char iface[STRING_SIZE];
	char locip[STRING_SIZE];
	char dns1[STRING_SIZE];
	char dns2[STRING_SIZE];
	char command[STRING_SIZE];
	int trys;
	
	if (!(initsetuid(0)))
		exit(1);
	
	system("/usr/bin/killall snort >/dev/null 2>/dev/null");

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

        if (!(ipfile = fopen("/var/smoothwall/red/local-ipaddress", "r")))
        {
                fprintf(stderr, "Couldn't open local ip file\n");
                exit(0);
        }
        fgets(locip, STRING_SIZE, ipfile);
        if (locip[strlen(locip) - 1] == '\n')
                locip[strlen(locip) - 1] = '\0';
        fclose (ipfile);
        ipfile = NULL;	
        if (strspn(locip, IP_NUMBERS) != strlen(locip))
	{
		fprintf(stderr, "Bad local IP: %s\n", locip);
		exit(0);
	}
	
        if (!(dns1file = fopen("/var/smoothwall/red/dns1", "r")))
        {
                fprintf(stderr, "Couldn't open dns1 file\n");
                exit(0);
        }
        fgets(dns1, STRING_SIZE, dns1file);
        if (dns1[strlen(dns1) - 1] == '\n')
                dns1[strlen(dns1) - 1] = '\0';
        fclose (dns1file);
        dns1file = NULL;
        if (strspn(dns1, IP_NUMBERS) != strlen(dns1))
	{
		fprintf(stderr, "Bad DNS1 IP: %s\n", dns1);
		exit(0);
	}
	        
        if (!(dns2file = fopen("/var/smoothwall/red/dns2", "r")))
        {
                fprintf(stderr, "Couldn't open dns2 file\n");
                exit(0);
        }
        fgets(dns2, STRING_SIZE, dns2file);
        if (dns2[strlen(dns2) - 1] == '\n')
                dns2[strlen(dns2) - 1] = '\0';
        fclose (dns2file);
        dns2file = NULL;
        if (strspn(dns2, IP_NUMBERS) != strlen(dns2))
	{
		fprintf(stderr, "Bad DNS2 IP: %s\n", dns2);
		exit(0);
	}
        
	if (!(varsfile = fopen("/etc/snort/vars", "w")))
	{
		fprintf(stderr, "Couldn't create vars file\n");
		exit(0);
	}
	fprintf(varsfile, "var HOME_NET %s\n", locip);
	if (strlen(dns1))
	{
		if (strlen(dns2))
			fprintf(varsfile, "var DNS_SERVERS [%s,%s]\n", dns1, dns2);
		else
			fprintf(varsfile, "var DNS_SERVERS %s\n", dns1);
	}

	fclose(varsfile);
	varsfile = NULL;
	
        if ((fd = open("/var/smoothwall/snort/enable", O_RDONLY)) != -1)
        {
        	close(fd);
        	snprintf(command, STRING_SIZE -1,
        		"/usr/bin/snort -c /etc/snort.conf -D -u snort -g snort -d -e -z -A Full -i %s",
        		iface);
               	system(command);
               	for (trys = 0; trys < 5; trys++)
               	{
               		sleep(2);
               		/* Silly snort creates pids with mode 600 */
               		sprintf(command, "/var/run/snort_%s.pid", iface);
               			if ((fd = open(command, 0)) == -1)
               		{
				break;
               		}
               	}
               	if (fd != -1)
               	{
	               	fchmod(fd, 00644);
	               	close(fd);
	        }
	}

	return 0;
}

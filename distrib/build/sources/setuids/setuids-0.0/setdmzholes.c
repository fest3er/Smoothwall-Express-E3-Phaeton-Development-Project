/* SmoothWall helper program - setdmzhole
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * (c) Daniel Goscomb, 2001
 * 
 * Modifications and improvements by Lawrence Manning.
 *
 * 10/04/01 Aslak added protocol support
 *
 * filename: setdmzhole.c
 * This program reads the list of ports to forward and setups ipchains
 * and rules in ipmasqadm to enable them. */

#include "setuid.h"

int main(void)
{
	FILE *fwdfile = NULL;
	int count;
	char *protocol;
	char *locip;
	char *remip;
	char *remport;
	char *enabled;
	char s[STRING_SIZE];
	char *result;
	char command[STRING_SIZE];

	if (!(initsetuid(1)))
		exit(1);
			
 	if (!(fwdfile = fopen("/var/smoothwall/dmzholes/config", "r")))
	{
		fprintf(stderr, "Couldn't open dmzholes settings file");
		goto EXIT;
	}

	system("/sbin/iptables -F dmzholes");

	while (fgets(s, STRING_SIZE, fwdfile) != NULL)
	{
		if (s[strlen(s) - 1] == '\n')
		        s[strlen(s) - 1] = '\0';
		result = strtok(s, ",");
		
		count = 0;
		protocol = NULL;
		locip = NULL; remip = NULL;
		remport = NULL;
		enabled = NULL;
		while (result)
		{
			if (count == 0)
				protocol = result;
			else if (count == 1)
				locip = result;
			else if (count == 2)
				remip = result;
			else if (count == 3)
				remport = result;
			else
				enabled = result;
			count++;
			result = strtok(NULL, ",");
		}

		if (!(protocol && locip && remip && remport && enabled))
			break;

		if (strspn(protocol, LETTERS) != strlen(protocol))
		{
			fprintf(stderr, "Bad protocol: %s\n", protocol);
			goto EXIT;
		}
		if (strspn(locip, IP_NUMBERS) != strlen(locip))
		{
			fprintf(stderr, "Bad local IP: %s\n", locip);
			goto EXIT;
		}
		if (strspn(remip, IP_NUMBERS) != strlen(remip))
		{
			fprintf(stderr, "Bad remote IP: %s\n", remip);
			goto EXIT;
		}
		if (strspn(remport, NUMBERS) != strlen(remport))
		{
			fprintf(stderr, "Bad remote port: %s\n", remport);
			goto EXIT;
		}
		
		if (strcmp(enabled, "on") == 0)
		{
			memset(command, 0, STRING_SIZE);
			snprintf(command, STRING_SIZE - 1, "/sbin/iptables -A dmzholes -m state --state NEW -p %s -s %s -d %s --dport %s -j ACCEPT", protocol, locip, remip, remport);
			system(command);
		}
	}
		
EXIT:
	if (fwdfile)
		fclose(fwdfile);
	
	return 0;
}

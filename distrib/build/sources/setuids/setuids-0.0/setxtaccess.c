/* SmoothWall helper program - setxtaccess
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
 * filename: setxtaccess.c */
 
#include "setuid.h"

int main(void)
{
	FILE *ifacefile = NULL;
	FILE *fwdfile = NULL;
	int count;
	char iface[STRING_SIZE] = "";
	char *protocol;
	char *remip;
	char *locport;
	char *enabled;
	char s[STRING_SIZE];
	char *result;
	char command[STRING_SIZE];

	if (!(initsetuid(1)))
		exit(1);

	if ((ifacefile = fopen("/var/smoothwall/red/iface", "r")))
	{
		fgets(iface, STRING_SIZE, ifacefile);
		if (iface[strlen(iface) - 1] == '\n')
			iface[strlen(iface) - 1] = '\0';	
	 	fclose (ifacefile);
	 	ifacefile = NULL;
	}
	if ((strncmp(iface, "ppp", 3) == 0) || (strncmp(iface, "ippp", 4) == 0))
		strcpy(iface, "");
        if (strspn(iface, LETTERS_NUMBERS) != strlen(iface))
	{
		fprintf(stderr, "Bad iface: %s\n", iface);
		goto EXIT;
	}
 	
 	if (!(fwdfile = fopen("/var/smoothwall/xtaccess/config", "r")))
	{
		fprintf(stderr, "Couldn't open xtaccess settings file");
		goto EXIT;
	}

	system("/sbin/iptables -F xtaccess");

	while (fgets(s, STRING_SIZE, fwdfile) != NULL)
	{
		if (s[strlen(s) - 1] == '\n')
		        s[strlen(s) - 1] = '\0';		        	
		result = strtok(s, ",");
		count = 0;
		protocol = NULL;
		remip = NULL;
		locport = NULL;
		enabled = NULL;
		while (result)
		{
			if (count == 0)
				protocol = result;
			else if (count == 1)
				remip = result;
			else if (count == 2)
				locport = result;
			else
				enabled = result;
			count++;
			result = strtok(NULL, ",");
		}

		if (!(protocol && remip && locport && enabled))
			break;
		
		if (strspn(protocol, LETTERS) != strlen(protocol))
		{
			fprintf(stderr, "Bad protocol: %s\n", protocol);
			goto EXIT;
		}
		if (strspn(remip, IP_NUMBERS) != strlen(remip))
		{
			fprintf(stderr, "Bad remote IP: %s\n", remip);
			goto EXIT;
		}
		if (strspn(locport, NUMBERS) != strlen(locport))
		{
			fprintf(stderr, "Bad local port: %s\n", locport);
			goto EXIT;
		}	

		if (strcmp(enabled, "on") == 0)
		{
			memset(command, 0, STRING_SIZE);
			snprintf(command, STRING_SIZE - 1, "/sbin/iptables -A xtaccess -i ppp0 -p %s -s %s --destination-port %s -j ACCEPT",
				protocol, remip, locport);
			system(command);

			memset(command, 0, STRING_SIZE);
			snprintf(command, STRING_SIZE - 1, "/sbin/iptables -A xtaccess -i ippp0 -p %s -s %s --destination-port %s -j ACCEPT",
				protocol, remip, locport);
			system(command);

			if (strlen(iface))
			{
				memset(command, 0, STRING_SIZE);
				snprintf(command, STRING_SIZE - 1, "/sbin/iptables -A xtaccess -i %s -p %s -s %s --destination-port %s -j ACCEPT",
					iface, protocol, remip, locport);
				system(command);
			}
		}
	}
		
EXIT:
	if (ifacefile)
		fclose(ifacefile);
	if (fwdfile)
		fclose(fwdfile);
	
	return 0;
}

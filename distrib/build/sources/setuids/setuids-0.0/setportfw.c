/* SmoothWall helper program - restartdhcp
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
 * filename: setportfwd.c
 * This program reads the list of ports to forward and setups ipchains
 * and rules in ipmasqadm to enable them. */

#include "setuid.h"

int main(void)
{
	FILE *ipfile = NULL, *ifacefile = NULL;
	FILE *fwdfile = NULL;
	int count;
	char iface[STRING_SIZE];
	char locip[STRING_SIZE];
	char *protocol;
	char *extip;
	char *locport;
	char *remip;
	char *remport;
	char *origremport;
	char *enabled;
	char s[STRING_SIZE];
	char *result;
	char command[STRING_SIZE];
	char remdouble[STRING_SIZE];

	if (!(initsetuid(1)))
		exit(1);
	
	if (!(ipfile = fopen("/var/smoothwall/red/local-ipaddress", "r")))
	{
		fprintf(stderr, "Couldn't open local ip file");
		goto EXIT;
	}
	fgets(locip, STRING_SIZE, ipfile);
	if (locip[strlen(locip) - 1] == '\n')
		locip[strlen(locip) - 1] = '\0';
 	fclose (ipfile);
 	ipfile = NULL; 	
 	if (strspn(locip, IP_NUMBERS) != strlen(locip))
 	{
 		fprintf(stderr, "Bad local IP: %s\n", locip);
 		goto EXIT;
 	}
 	
	if (!(ifacefile = fopen("/var/smoothwall/red/iface", "r")))
	{
		fprintf(stderr, "Couldn't open iface file");
		goto EXIT;
	}
	fgets(iface, STRING_SIZE, ifacefile);
	if (iface[strlen(iface) - 1] == '\n')
		iface[strlen(iface) - 1] = '\0';	
 	fclose (ifacefile);
 	ifacefile = NULL;
 	if (strspn(iface, LETTERS_NUMBERS) != strlen(iface))
 	{
 		fprintf(stderr, "Bad iface: %s\n", iface);
 		goto EXIT;
 	}
 	
 	if (!(fwdfile = fopen("/var/smoothwall/portfw/config", "r")))
	{
		fprintf(stderr, "Couldn't open portfw settings file\n");
		goto EXIT;
	}

	system("/sbin/iptables -F portfwf");
	system("/sbin/iptables -t nat -F portfw");

	while (fgets(s, STRING_SIZE, fwdfile) != NULL)
	{
		if (s[strlen(s) - 1] == '\n')
		        s[strlen(s) - 1] = '\0';		        	
		result = strtok(s, ",");

		count = 0;
		protocol = NULL;
		extip = NULL;
		locport = NULL;
		remip = NULL;
		remport = NULL;
		enabled = NULL;
		while (result)
		{
			if (count == 0)
				protocol = result;
			else if (count == 1)
				extip = result;
			else if (count == 2)
				locport = result;
			else if (count == 3)
				remip = result;
			else if (count == 4)
				remport = result;
			else
				enabled = result;
			count++;
			result = strtok(NULL, ",");
		}
		
		if (!(protocol && extip && locport && remip && remport && enabled))
			break;
		
		if (strspn(protocol, LETTERS) != strlen(protocol))
		{
			fprintf(stderr, "Bad protocol: %s\n", protocol);
			goto EXIT;
		}
		if (strspn(extip, IP_NUMBERS) != strlen(extip))
		{
			fprintf(stderr, "Bad external IP: %s\n", extip);
			goto EXIT;
		}
		if (strspn(locport, NUMBERS_COLON) != strlen(locport))
		{
			fprintf(stderr, "Bad loal port: %s\n", locport);
			goto EXIT;
		}

		if (strspn(remport, NUMBERS) != strlen(remport))
		{
			fprintf(stderr, "Bad remote port: %s\n", remport);
			goto EXIT;
		}
		if (strcmp(enabled, "on") == 0)
		{
			origremport = remport;
 
			if (!atol(remport))
				remport = locport;
		
			if (!strchr(locport, ':') || atol(origremport))
			{
				snprintf(remdouble, STRING_SIZE - 1, "%s:%s",
					remip, remport);
			}
			else
				strncpy(remdouble, remip, STRING_SIZE - 1);	

			memset(command, 0, STRING_SIZE);
			snprintf(command, STRING_SIZE - 1,
				"/sbin/iptables -A portfwf -m state --state NEW -i %s -p %s -s %s -d %s --dport %s -j ACCEPT",
					iface, protocol, extip, remip, remport);
			system(command);
			
			snprintf(command, STRING_SIZE - 1,
				"/sbin/iptables -t nat -A portfw -p %s -s %s -d %s --dport %s -j DNAT --to %s",
					protocol, extip, locip, locport, remdouble);
			system(command);			
		}
	}
		
EXIT:
	if (ipfile)
		fclose(ipfile);
	if (ifacefile)
		fclose(ifacefile);
	if (fwdfile)
		fclose(fwdfile);
	
	return 0;
}

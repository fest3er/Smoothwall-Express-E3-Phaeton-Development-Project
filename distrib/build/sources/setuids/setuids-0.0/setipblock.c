/* SmoothWall helper program - setipblock
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * (c) Lawrence Manning, 2003
 * 
 * filename: setipblock.c
 * Sets up the IP block rules. */

#include "setuid.h"

int main(void)
{
	FILE *fwdfile = NULL;
	int count = 0;
	char *remip = NULL;
	char *log = NULL;
	char *target = NULL;
	char *enabled = NULL;
	char s[STRING_SIZE];
	char *result = NULL;
	char command[STRING_SIZE];

	memset(s, 0, STRING_SIZE);
	memset(command, 0, STRING_SIZE);

	if (!(initsetuid(1)))
		goto EXIT;
			
 	if (!(fwdfile = fopen("/var/smoothwall/ipblock/config", "r")))
	{
		fprintf(stderr, "Couldn't open dmzholes settings file");
		goto EXIT;
	}

	system("/sbin/iptables -F ipblock");

	while (fgets(s, STRING_SIZE, fwdfile) != NULL)
	{
		if (s[strlen(s) - 1] == '\n')
			s[strlen(s) - 1] = '\0';

		result = strtok(s, ",");
		
		count = 0;
		remip = NULL;
		log = NULL;
		target = NULL;
		enabled = NULL;
		while (result)
		{
			if (count == 0)
				remip = result;
			else if (count == 1)
				log = result;
			else if (count == 2)
				target = result;
			else if (count == 3)
				enabled = result;
			count++;
			result = strtok(NULL, ",");
		}

		if (!(remip && enabled))
			break;

		if (strspn(remip, IP_NUMBERS) != strlen(remip))
		{
			fprintf(stderr, "Bad remote IP: %s\n", remip);
			goto EXIT;
		}
		if (!(strcmp(target, "DROP") == 0 || strcmp(target, "REJECT") == 0))
		{
			fprintf(stderr, "Bad target: %s\n", target);
			goto EXIT;
		}

		if (strcmp(enabled, "on") == 0)
		{
			if (strcmp(log, "on") == 0)
			{
				snprintf(command, STRING_SIZE, "/sbin/iptables -A ipblock -s %s -j LOG", remip);
				system(command);
			}

			snprintf(command, STRING_SIZE, "/sbin/iptables -A ipblock -s %s -j %s", remip, target);
			system(command);
		}
	}
		
EXIT:
	if (fwdfile)
		fclose(fwdfile);

	return 0;
}

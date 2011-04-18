/* SmoothWall helper program - installpackage
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * (c) Lawrence Manning, 2001
 * filename: installpackage.c */
          
#include "setuid.h"

int main(int argc, char *argv[])
{
	char command[STRING_SIZE];
	int ret;
	FILE *p;
	char buffer[STRING_SIZE];

	if (!(initsetuid(1)))
		exit(1);
	
	/* Sanatise arguments */
	if (argc < 2)
	{
		fprintf(stderr, "Missing arg\n");
		exit(1);
	}
	if (strspn(argv[1], NUMBERS) != strlen(argv[1]))
	{
		fprintf(stderr, "Bad arg\n");
		exit(1);
	}
	
	openlog("installpackage", LOG_PID, LOG_USER);
	
	memset(command, 0, STRING_SIZE);
	snprintf(command, STRING_SIZE - 1, "/var/patches/%s/setup", argv[1]);
	if (!(p = popen(command, "r")))
		return -1;
	setvbuf(p, NULL, _IOLBF, 255);	
	
	while (fgets(buffer, STRING_SIZE, p))
	{
		syslog(LOG_INFO, buffer);
	}
				
	ret = pclose(p);
	closelog();

	memset(command, 0, STRING_SIZE);
	snprintf(command, STRING_SIZE - 1, "/bin/rm -rf /var/patches/%s", argv[1]);
	system(command);
	
	return ret;
}

/* SmoothWall helper program - listtables
 *
 * Directly pinched from CS3.0. */
 
#include "setuid.h"

int main(int argc, char *argv[])
{
	int fd = -1;
	char *table = NULL;
	pid_t pid = -1;
	char buffer[STRING_SIZE];
	FILE *hpipe = NULL;

	if (!(initsetuid(1)))
		goto EXIT;

	if (argc < 2)
		table = "filter";
	else
		table = argv[1];
	
	fd = simplesecurepopenl(&pid, "/sbin/iptables", "-t", table, "-L", "-nv", NULL);
	if (fd != -1)
	{
		hpipe = fdopen(fd, "r");
		while (fgets(buffer, STRING_SIZE, hpipe))
			printf(buffer);	
	}

EXIT:
	if (hpipe)
		fclose(hpipe);
	if (fd != -1)
		close(fd);
	
	cleanupsetuid();
	
	return 0;
}

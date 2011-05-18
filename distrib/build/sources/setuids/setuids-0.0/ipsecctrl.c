#include "setuid.h"

int main(int argc, char *argv[])
{
	if (!(initsetuid(1)))
		exit(1);
	
	if (argc < 2)
	{
		fprintf(stderr, "Missing arg\n");
		exit(1);
	}
	
	if (strcmp(argv[1], "S") == 0)
	{
		system("/usr/sbin/ipsec setup stop >/dev/null");
		exit(0);
	}
	else if (strcmp(argv[1], "R") == 0)
		system("/usr/sbin/ipsec setup restart >/dev/null");	
	else
	{
		fprintf(stderr, "Bad arg\n");
		exit(1);
	}

	sleep(4);
	
	return 0;
}
